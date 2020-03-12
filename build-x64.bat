@echo off
@chcp 1251 & @rem https://ru.stackoverflow.com/a/459299/288575

@rem ���樠������ Microsoft Visual Studio 2017
@call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\Tools\VsDevCmd.bat"
@rem @set gtest_root=d:\GIT_REPOSITORIES\open-source-libs\gtest-1.8.0\googletest
@echo ** gtest_root=%gtest_root%

@%~d0
@cd %~dp0

@if exist build @rd /S /Q build
@if not exist build @mkdir build
@cd build
@cmake -G"Visual Studio 15 2017 Win64" ..
@rem cmake -G"Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Debug ..

@:eof
@pause
