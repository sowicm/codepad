@echo off

set TARGET_DIR=%ProgramFiles%
set COPY_WXC_RESOURCES=0
::IF EXIST D:\software\NUL (set TARGET_DIR=D:\software)
IF EXIST ..\wxcrafter\wxcrafter.accelerators (set COPY_WXC_RESOURCES=1)

xcopy config\*.default "%TARGET_DIR%\Sowicm's Codepad\config\" /E /I /H /Y /EXCLUDE:excludes
xcopy config\build_settings.xml.default.win "%TARGET_DIR%\Sowicm's Codepad\config\build_settings.xml.default" /E /I /H /Y /EXCLUDE:excludes
xcopy debuggers\*.dll "%TARGET_DIR%\Sowicm's Codepad\debuggers\" /E /I /H /Y /EXCLUDE:excludes
xcopy images\* "%TARGET_DIR%\Sowicm's Codepad\images\" /E /I /H /Y /EXCLUDE:excludes
xcopy lexers\*.json "%TARGET_DIR%\Sowicm's Codepad\lexers\" /E /I /H /Y /EXCLUDE:excludes
xcopy plugins\*.dll "%TARGET_DIR%\Sowicm's Codepad\plugins\" /E /I /H /Y /EXCLUDE:excludes
xcopy ..\SpellChecker\dics\* "%TARGET_DIR%\Sowicm's Codepad\dics\" /E /I /H /Y /EXCLUDE:excludes
xcopy lib*.dll "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes
xcopy plugins\resources\*.* "%TARGET_DIR%\Sowicm's Codepad\plugins\resources\" /E /I /H /Y /EXCLUDE:excludes
xcopy rc\*.xrc "%TARGET_DIR%\Sowicm's Codepad\rc\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\ "%TARGET_DIR%\Sowicm's Codepad\templates\" /E /I /H /Y /EXCLUDE:excludes
xcopy gdb_printers\*.py "%TARGET_DIR%\Sowicm's Codepad\gdb_printers\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\formbuilder\* "%TARGET_DIR%\Sowicm's Codepad\templates\formbuilder\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\projects\* "%TARGET_DIR%\Sowicm's Codepad\templates\projects\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\gizmos\* "%TARGET_DIR%\Sowicm's Codepad\templates\gizmos\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\codedesigner\* "%TARGET_DIR%\Sowicm's Codepad\templates\codedesigner\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\databaselayer\* "%TARGET_DIR%\Sowicm's Codepad\templates\databaselayer\" /E /I /H /Y /EXCLUDE:excludes
xcopy templates\qmake\* "%TARGET_DIR%\Sowicm's Codepad\templates\qmake\" /E /I /H /Y /EXCLUDE:excludes
xcopy *.html "%TARGET_DIR%\Sowicm's Codepad\" /H /Y /EXCLUDE:excludes
xcopy codelite-icons.zip "%TARGET_DIR%\Sowicm's Codepad\" /H /Y /EXCLUDE:excludes
xcopy codelite-bitmaps.zip "%TARGET_DIR%\Sowicm's Codepad\" /H /Y /EXCLUDE:excludes
xcopy codelite-icons-dark.zip "%TARGET_DIR%\Sowicm's Codepad\" /H /Y /EXCLUDE:excludes
xcopy codelite-icons-fresh-farm.zip "%TARGET_DIR%\Sowicm's Codepad\" /H /Y /EXCLUDE:excludes
xcopy locale\* "%TARGET_DIR%\Sowicm's Codepad\locale\" /E /I /H /Y /EXCLUDE:excludes
xcopy ..\lib\gcc_lib\libdatabaselayersqlite*.dll "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes
xcopy ..\lib\gcc_lib\libwxshapeframework*.dll "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes
IF EXIST wxgui.zip ( copy wxgui.zip "%TARGET_DIR%\Sowicm's Codepad\" )
IF EXIST PHP.zip ( copy PHP.zip "%TARGET_DIR%\Sowicm's Codepad\" )
IF EXIST ..\WebTools\javascript-win.zip ( copy ..\WebTools\javascript-win.zip "%TARGET_DIR%\Sowicm's Codepad\" )

if "%WXWIN%" == "" GOTO OTHERS
xcopy %WXWIN%\lib\gcc_dll\wxmsw*u_*gcc_cl.dll "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes
xcopy %WXWIN%\lib\gcc_dll\wxbase*u_*gcc_cl.dll "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes
:: xcopy %WXWIN%\lib\gcc_dll\wxrc.exe "%TARGET_DIR%\Sowicm's Codepad\" /E /I /H /Y /EXCLUDE:excludes

if "%COPY_WXC_RESOURCES%" == "1" (copy ..\wxcrafter\wxcrafter.accelerators  "%TARGET_DIR%\Sowicm's Codepad\plugins\resources" )

:OTHERS
:: Copy the misc files
copy codelite_indexer.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy codelite-cc.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy codelite_cppcheck.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy codelite_launcher.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy codelite-echo.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy ..\sdk\clang\lib\libclang64.dll "%TARGET_DIR%\Sowicm's Codepad\libclang.dll" /Y
copy ..\sdk\clang\lib\clang-format-64.exe "%TARGET_DIR%\Sowicm's Codepad\codelite-clang-format.exe" /Y
copy ..\sdk\libssh\lib\libssh64.dll "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy makedir.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy le_exec.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy CodeLite.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy codelite-make.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy Codepad-Terminal.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy patch.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy rm.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy astyle.sample "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy pthreadGC2.dll "%TARGET_DIR%\Sowicm's Codepad\" /Y
copy ..\sdk\wxconfig\wx-config.exe "%TARGET_DIR%\Sowicm's Codepad\" /Y

:END

echo codelite was updated into %TARGET_DIR%\Sowicm's Codepad
