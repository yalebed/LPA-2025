@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

:: Компиляция с генерацией отладочных символов
ml /c /nologo /Zi /Fo program.obj in.txt.asm
if errorlevel 1 (
    echo Compilation failed. Check your ASM code for errors.
    exit /b 1
)

:: Линковка с генерацией PDB (отладочный файл)
link /nologo /DEBUG /subsystem:console program.obj "D:\bstu\3sem\lpa2\lpaa\lpa\LPA-2025\Debug\LIB.lib" libucrt.lib libcmt.lib libvcruntime.lib kernel32.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD /PDB:program.pdb
if errorlevel 1 (
    echo Linking failed. Check library paths and compatibility.
    exit /b 1
)
program.exe

exit /b 0