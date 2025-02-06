@echo off
Portable.RemoteTasks.Manager.exe ^
-ul u472446 ^
-up 62462b19-2cb8-48c3-90ce-572a8952379a ^
-s ExecuteBinaryWithInteractiveInput ^
definitionFile target.pdsl ^
archName DynamicVM ^
asmListing test.asm ^
binaryFileToRun util/assmfile ^
finishMnemonicName hlt ^
codeRamBankName data_bank ^
ipRegStorageName PC ^
stdinRegStName iv ^
stdoutRegStName ov ^
-r stdout.txt > util/out