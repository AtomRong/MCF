@echo off

set Config=Debug64
set CFlags=-fno-builtin -g -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -O0
if "%1"=="Release" (
	set Config=Release64
	set CFlags=-DNDEBUG -O3 -flto -ffunction-sections -fdata-sections -Wl,--gc-sections,-s
)

set Lib=-lmcf -lmcfcrt -lgcc -lgcc_eh -lmcfcrt -lmingw32 -lmingwex -lgcc -lmcfcrt -lkernel32 -lntdll -luser32 -lshell32 -ladvapi32

call mingw 64

mcfbuild -p../../MCFCRT/MCFBuild.mcfproj -s../../MCFCRT -d../../.Built/%Config%/mcfcrt -o../../.Built/%Config%/libmcfcrt.a %* || exit /b 1
mcfbuild -p../../MCF/MCFBuild.mcfproj -s../../MCF -d../../.Built/%Config%/mcf -o../../.Built/%Config%/libmcf.a %* || exit /b 1

gcc %CFlags% -std=c11 -Wstrict-prototypes -Wall -Wextra -Wconversion -Wsign-conversion -pedantic -pedantic-errors -Wsign-conversion -Wsuggest-attribute=noreturn -pipe -mfpmath=both -march=nocona -mno-stack-arg-probe -mno-accumulate-outgoing-args -mpush-args -masm=intel main.c -o ".%Config%.exe" -I../.. -L../../.Built/%Config% -static -nostdlib -Wl,-e__MCFCRT_ExeStartup,--disable-runtime-pseudo-reloc,--disable-auto-import %Lib% || exit /b 1
