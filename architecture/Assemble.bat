@echo off

setlocal EnableDelayedExpansion

set Portable.RemoteTasks.Manager.Login=u472446
set Portable.RemoteTasks.Manager.Password=62462b19-2cb8-48c3-90ce-572a8952379a

Portable.RemoteTasks.Manager.exe -w -id -s Assemble definitionFile architecture.pdsl archName DynamicVM asmListing test.asm > util\tmp.txt
for /F %%i in (util\tmp.txt) do set id=%%i
echo TaskID: !id!

echo. 
echo RESULTS:
Portable.RemoteTasks.Manager.exe -g !id!
Portable.RemoteTasks.Manager.exe -g !id! -r out.ptptb > util\assmfile