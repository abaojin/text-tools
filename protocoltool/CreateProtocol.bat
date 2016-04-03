set project_home=%cd%

@echo off

setlocal enabledelayedexpansion 

set classpath=
for %%f in (./lib/*.jar) do (
	set onefile=%%f
	set classpath=!classpath!;%project_home%/lib/!onefile!
)

@echo on

echo %classpath%

set p_classpath=%CLASSPATH%;%project_home%/bin;%classpath%
java -classpath "p_classpath" "MessageMain"

pause