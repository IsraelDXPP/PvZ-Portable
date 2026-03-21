/*
 * Portions of this file are based on the PopCap Games Framework
 * Copyright (C) 2005-2009 PopCap Games, Inc.
 * 
 * Copyright (C) 2026 Zhou Qiankang <wszqkzqk@qq.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later AND LicenseRef-PopCap
 *
 * This file is part of PvZ-Portable.
 *
 * PvZ-Portable is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PvZ-Portable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PvZ-Portable. If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <filesystem>
#include "Common.h"
#include "PakInterface.h"
#include "fcaseopen/fcaseopen.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

enum
{
	FILEFLAGS_END = 0x80
};

class BinaryReader
{
	const uint8_t* mData;
	size_t mSize;
	size_t mPos;

public:
	BinaryReader(const uint8_t* theData, size_t theSize) : mData(theData), mSize(theSize), mPos(0) {}
	
	size_t Remaining() const { return mSize - mPos; }
	size_t GetPos() const { return mPos; }
	const uint8_t* GetDataPtr() const { return mData + mPos; }
	void Skip(size_t n) { mPos += n; }

	uint8_t ReadU8() { return mData[mPos++]; }
	uint32_t ReadU32() {
		uint32_t val = *(uint32_t*)(mData + mPos);
		mPos += 4;
		return Sexy::FromLE32(val);
	}
	uint64_t ReadU64() {
		uint64_t val = *(uint64_t*)(mData + mPos);
		mPos += 8;
		return Sexy::FromLE64(val);
	}
};

PakInterface* gPakInterface = new PakInterface();

PakInterface::PakInterface()
{
}

PakInterface::~PakInterface()
{
}

// Normalize path for pak lookup.
std::string PakInterface::NormalizePakPath(std::string_view theFileName)
{
	std::filesystem::path aFilePath = Sexy::PathFromU8(std::string(theFileName));

	// Make absolute paths relative to resource folder.
	if (aFilePath.has_root_directory())
	{
		const std::string& aResourceFolder = Sexy::GetResourceFolder();
		if (!aResourceFolder.empty())
		{
			std::filesystem::path aResPath = Sexy::PathFromU8(aResourceFolder);
			auto [aResEnd, aFileIt] = std::mismatch(aResPath.begin(), aResPath.end(), 
			                                       aFilePath.begin(), aFilePath.end());
			if (aResEnd == aResPath.end())
			{
				std::filesystem::path aRelativePath;
				for (; aFileIt != aFilePath.end(); ++aFileIt)
					aRelativePath /= *aFileIt;
				aFilePath = aRelativePath;
			}
		}
	}

	std::string aResult = Sexy::PathToU8(aFilePath.lexically_normal());
	
	if (aResult.size() >= 2 && aResult[0] == '.' && aResult[1] == '/')
		aResult = aResult.substr(2);
	
	std::transform(aResult.begin(), aResult.end(), aResult.begin(),
	               [](unsigned char c) { return std::toupper(c); });
	
	return aResult;
}

// Internal helper to parse pak data from a BinaryReader
bool PakInterface::ParsePakData(PakCollection* aPakCollection, BinaryReader& aReader, const std::string& aPakKey)
{
	uint32_t aMagic = aReader.ReadU32();
	if (aMagic != 0xBAC04AC0)
		return false;

	uint32_t aVersion = aReader.ReadU32();
	if (aVersion > 0)
		return false;

	int aPos = 0;
	for (;;)
	{
		if (aReader.Remaining() < 1) break;
		uint8_t aFlags = aReader.ReadU8();
		if (aFlags & FILEFLAGS_END)
			break;

		if (aReader.Remaining() < 1) break;
		uint8_t aNameWidth = aReader.ReadU8();
		if (aReader.Remaining() < aNameWidth) break;
		
		char aNameBuffer[256];
		memcpy(aNameBuffer, aReader.GetDataPtr(), aNameWidth);
		aNameBuffer[aNameWidth] = 0;
		aReader.Skip(aNameWidth);

		if (aReader.Remaining() < 4) break;
		int aSrcSize = static_cast<int>(aReader.ReadU32());
		if (aReader.Remaining() < 8) break;
		int64_t aFileTime = static_cast<int64_t>(aReader.ReadU64());

		for (int i = 0; i < aNameWidth; i++)
		{
			if (aNameBuffer[i] == '\\')
				aNameBuffer[i] = '/';
		}

		std::string aKey = NormalizePakPath(aNameBuffer);
		auto aRecItr = mPakRecordMap.emplace(aKey, PakRecord()).first;
		PakRecord* aRec = &aRecItr->second;
		aRec->mCollection = aPakCollection;
		aRec->mFileName = aKey;
		aRec->mStartPos = aPos;
		aRec->mSize = aSrcSize;
		aRec->mFileTime = aFileTime;

		aPos += aSrcSize;
	}

	int anOffset = aReader.GetPos();
	for (auto& [key, record] : mPakRecordMap)
	{
		if (record.mCollection == aPakCollection)
			record.mStartPos += anOffset;
	}
	return true;
}

bool PakInterface::AddPakFile(const std::string& theFileName)
{
	FILE *aFileHandle = fcaseopen(theFileName.c_str(), "rb");
	if (!aFileHandle)
		return false;

	fseek(aFileHandle, 0, SEEK_END);
	size_t aFileSize = ftell(aFileHandle);
	fseek(aFileHandle, 0, SEEK_SET);

	mPakCollectionList.emplace_back(aFileSize);
	PakCollection* aPakCollection = &mPakCollectionList.back();

	if (fread(aPakCollection->mDataPtr, 1, aFileSize, aFileHandle) != aFileSize)
	{
		fclose(aFileHandle);
		return false;
	}
	fclose(aFileHandle);

	auto *aDataPtr = static_cast<uint8_t *>(aPakCollection->mDataPtr);
	for (size_t i = 0; i < aFileSize; i++)
		*aDataPtr++ ^= 0xF7;

	std::string aPakKey = NormalizePakPath(theFileName);
	auto aRecordItr = mPakRecordMap.emplace(aPakKey, PakRecord()).first;
	PakRecord* aPakRecord = &aRecordItr->second;
	aPakRecord->mCollection = aPakCollection;
	aPakRecord->mFileName = aPakKey;
	aPakRecord->mStartPos = 0;
	aPakRecord->mSize = static_cast<int>(aFileSize);

	BinaryReader aReader(static_cast<uint8_t*>(aPakCollection->mDataPtr), aFileSize);
	return ParsePakData(aPakCollection, aReader, aPakKey);
}

bool PakInterface::AddPakMemory(void* theData, size_t theSize, bool theOwnsMemory, const std::string& theName)
{
	if (theData == nullptr || theSize == 0)
		return false;

	// Note: We always copy or ensure we have writable memory because we XOR in place
	void* aDataPtr = malloc(theSize);
	memcpy(aDataPtr, theData, theSize);

	mPakCollectionList.emplace_back(theSize);
	PakCollection* aPakCollection = &mPakCollectionList.back();
	free(aPakCollection->mDataPtr); // Replace default malloc
	aPakCollection->mDataPtr = aDataPtr;

	auto* aPtr = static_cast<uint8_t*>(aPakCollection->mDataPtr);
	for (size_t i = 0; i < theSize; i++)
		*aPtr++ ^= 0xF7;

	std::string aPakKey = NormalizePakPath(theName);
	auto aRecordItr = mPakRecordMap.emplace(aPakKey, PakRecord()).first;
	PakRecord* aPakRecord = &aRecordItr->second;
	aPakRecord->mCollection = aPakCollection;
	aPakRecord->mFileName = aPakKey;
	aPakRecord->mStartPos = 0;
	aPakRecord->mSize = static_cast<int>(theSize);

	// Parse from memory
	BinaryReader aReader(static_cast<uint8_t*>(aPakCollection->mDataPtr), theSize);
	return ParsePakData(aPakCollection, aReader, aPakKey);
}

//0x5D85C0
PFILE* PakInterface::FOpen(const char* theFileName, const char* anAccess)
{
	if ((strcasecmp(anAccess, "r") == 0) || (strcasecmp(anAccess, "rb") == 0) || (strcasecmp(anAccess, "rt") == 0))
	{
		std::string aKey = NormalizePakPath(theFileName);
		auto anItr = mPakRecordMap.find(aKey);
		if (anItr != mPakRecordMap.end())
		{
			PFILE* aPFP = new PFILE;
			aPFP->mRecord = &anItr->second;
			aPFP->mPos = 0;
			aPFP->mFP = nullptr;
			return aPFP;
		}
	}

	const std::string& aResourceBase = Sexy::GetResourceFolder();
	FILE* aFP = nullptr;
	if (!aResourceBase.empty() && !Sexy::PathFromU8(theFileName).has_root_directory())
	{
		aFP = fcaseopenat(aResourceBase.c_str(), theFileName, anAccess);
	}
	else
	{
		aFP = fcaseopen(theFileName, anAccess);
	}

	if (aFP == nullptr)
		return nullptr;

	PFILE* aPFP = new PFILE;
	aPFP->mRecord = nullptr;
	aPFP->mPos = 0;
	aPFP->mFP = aFP;
	return aPFP;
}

//0x5D8780
int PakInterface::FClose(PFILE* theFile)
{
	if (theFile->mRecord == nullptr)
		fclose(theFile->mFP);
	delete theFile;
	return 0;
}

//0x5D87B0
int PakInterface::FSeek(PFILE* theFile, long theOffset, int theOrigin)
{
	if (theFile->mRecord != nullptr)
	{
		if (theOrigin == SEEK_SET)
			theFile->mPos = theOffset;
		else if (theOrigin == SEEK_END)
			theFile->mPos = theFile->mRecord->mSize - theOffset;
		else if (theOrigin == SEEK_CUR)
			theFile->mPos += theOffset;

		// 当前指针位置不能超过整个文件的大小，且不能小于 0
		theFile->mPos = std::max(std::min(theFile->mPos, theFile->mRecord->mSize), 0);
		return 0;
	}
	else
		return fseek(theFile->mFP, theOffset, theOrigin);
}

//0x5D8830
int PakInterface::FTell(PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
		return theFile->mPos;
	else
		return ftell(theFile->mFP);	
}

//0x5D8850
size_t PakInterface::FRead(void* thePtr, int theElemSize, int theCount, PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
	{
		// 实际读取的字节数不能超过当前资源文件剩余可读取的字节数
		int aSizeBytes = std::min(theElemSize*theCount, theFile->mRecord->mSize - theFile->mPos);

		// 取得在整个 pak 中开始读取的位置的指针
		uchar* src = (uchar*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos;
		uchar* dest = (uchar*) thePtr;
		memcpy(dest, src, aSizeBytes);
		theFile->mPos += aSizeBytes;  // 读取完成后，移动当前读取位置的指针
		return aSizeBytes / theElemSize;  // 返回实际读取的项数
	}
	
	return fread(thePtr, theElemSize, theCount, theFile->mFP);	
}

int PakInterface::FGetC(PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
	{
		for (;;)
		{
			if (theFile->mPos >= theFile->mRecord->mSize)
				return EOF;		
			char aChar = *((char*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos++);
			if (aChar != '\r')
				return (uchar) aChar;
		}
	}

	return fgetc(theFile->mFP);
}

int PakInterface::UnGetC(int theChar, PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
	{
		// This won't work if we're not pushing the same chars back in the stream
		theFile->mPos = std::max(theFile->mPos - 1, 0);
		return theChar;
	}

	return ungetc(theChar, theFile->mFP);
}

char* PakInterface::FGetS(char* thePtr, int theSize, PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
	{
		int anIdx = 0;
		while (anIdx < theSize)
		{
			if (theFile->mPos >= theFile->mRecord->mSize)
			{
				if (anIdx == 0)
					return nullptr;
				break;
			}
			char aChar = *((char*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos++);
			if (aChar != '\r')
				thePtr[anIdx++] = aChar;
			if (aChar == '\n')
				break;
		}
		thePtr[anIdx] = 0;
		return thePtr;
	}

	return fgets(thePtr, theSize, theFile->mFP);
}

int PakInterface::FEof(PFILE* theFile)
{
	if (theFile->mRecord != nullptr)
		return theFile->mPos >= theFile->mRecord->mSize;
	else
		return feof(theFile->mFP);
}
