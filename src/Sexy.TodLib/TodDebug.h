/*
 * Copyright (C) 2026 Zhou Qiankang <wszqkzqk@qq.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
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

#ifndef __TODDEBUG_H__
#define __TODDEBUG_H__

class TodHesitationBracket
{
public:
	char			mMessage[256];
	int				mBracketStartTime;

public:
	TodHesitationBracket(const char* /*theFormat*/, ...) { ; }
	~TodHesitationBracket() { ; }

	inline void		EndBracket() { ; }
};

#ifdef _PVZ_DEBUG
void				TodLogImpl(const char* file, int line, const char* func, const char* theFormat, ...);
void				TodTraceImpl(const char* file, int line, const char* func, const char* theFormat, ...);
void				TodTraceAndLogImpl(const char* file, int line, const char* func, const char* theFormat, ...);
void				TodTraceWithoutSpammingImpl(const char* file, int line, const char* func, const char* theFormat, ...);

#define TodLog(fmt, ...) TodLogImpl(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define TodTrace(fmt, ...) TodTraceImpl(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define TodTraceAndLog(fmt, ...) TodTraceAndLogImpl(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define TodTraceWithoutSpamming(fmt, ...) TodTraceWithoutSpammingImpl(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
void				TodLog(const char* theFormat, ...);
void				TodTrace(const char* theFormat, ...);
void				TodTraceAndLog(const char* theFormat, ...);
void				TodTraceWithoutSpamming(const char* theFormat, ...);
#endif

void				TodLogString(const char* theMsg);
void				TodTraceMemory();
void				TodHesitationTrace(...);
void				TodAssertFailed(const char* theCondition, const char* theFile, int theLine, const char* theMsg = "", ...);
/*inline*/ void		TodErrorMessageBox(const char* theMessage, const char* theTitle);

/*inline*/ void*	TodMalloc(int theSize);
/*inline*/ void		TodFree(void* theBlock);
void				TodAssertInitForApp();

#ifdef _PVZ_DEBUG
#define TOD_ASSERT(condition, ...) { \
if (!bool(condition)) { TodAssertFailed(""#condition, __FILE__, __LINE__, ##__VA_ARGS__); \
/*if (IsDebuggerPresent()) { __debugbreak(); }*/\
TodTraceMemory(); }\
}
#else
#define TOD_ASSERT(condition, ...)
#endif

#endif
