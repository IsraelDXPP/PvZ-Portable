#if !defined(_WIN32) && !defined(__SWITCH__) && !defined(__3DS__) && !defined(__ANDROID__) && !defined(__IPHONEOS__) && !defined(__EMSCRIPTEN__)

#if defined(__APPLE__)
.section __TEXT,__const
.global _main_pak_data
_main_pak_data:
.incbin "main.pak"
.global _main_pak_size
_main_pak_size:
.quad _main_pak_size_end - _main_pak_data
_main_pak_size_end:
#else
.section .rodata
.global main_pak_data
.type main_pak_data, @object
main_pak_data:
.incbin "main.pak"
.global main_pak_size
.type main_pak_size, @object
main_pak_size:
.quad main_pak_size_end - main_pak_data
main_pak_size_end:
#endif

#endif
