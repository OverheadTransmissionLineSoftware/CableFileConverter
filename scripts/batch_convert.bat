@echo off

REM This script process multiple cable files at once.

REM captures command line arguments
SET PATH_APP=%1
SET UNITS=%2
SET DIR_INPUT=%3
SET DIR_OUTPUT=%4

REM creates a loop that iterates over all input directory files
FOR %%i in (%DIR_INPUT%\*) DO (
  %PATH_APP% -v --units=%UNITS% %%~i %DIR_OUTPUT%\%%~ni.cable
)
