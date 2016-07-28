@REM Copyright (c) Microsoft. All rights reserved.
@REM Licensed under the MIT license. See LICENSE file in the project root for full license information.

setlocal

set build-root=%~dp0..
rem // resolve to fully qualified path
for %%i in ("%build-root%") do set build-root=%%~fi

REM -- C --
cd %build-root%

mkdir %build-root%\cmake
pushd %build-root%\cmake
if errorlevel 1 goto :eof

cmake ..
if errorlevel 1 goto :eof

msbuild /m ctest.sln /p:Configuration=Release
if errorlevel 1 goto :eof
msbuild /m ctest.sln /p:Configuration=Debug
if errorlevel 1 goto :eof

popd
