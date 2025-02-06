@echo off
setlocal EnableDelayedExpansion
IF "%~1" neq "" (
	set Portable.RemoteTasks.Manager.Login=u472446
	set Portable.RemoteTasks.Manager.Password=62462b19-2cb8-48c3-90ce-572a8952379a
	set defFile=target.pdsl
	set archName=DynamicVM
	set asmListing=test.asm
	echo %Portable.RemoteTasks.Manager.Login%
	echo %Portable.RemoteTasks.Manager.Password%
	echo !archName!
	IF "%~1" == "test1" (
		Portable.RemoteTasks.Manager.exe -w -id -s AssembleRaw asmListing !asmListing! definitionFile !defFile! archName !archName! > util\tmp.txt
		for /F %%i in (util\tmp.txt) do set id=%%i

		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r raw.out.bin > util\out1.bin

		Portable.RemoteTasks.Manager.exe -w -id -s DisassembleRaw raw.in.bin util\out1.bin definitionFile !defFile! archName !archName! > util\tmp.txt
		
		for /F %%i in (util\tmp.txt) do set id=%%i
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r disasmListing.txt  > util\l1.out.asm

		objdump.exe -D -b binary -mi386 -Maddr32,data32 util\assfileraw > util\l2.out.asm

		

		exit /b
	
	)

	IF "%~1" == "assm" (
		Portable.RemoteTasks.Manager.exe -w -id -s Assemble asmListing !asmListing! definitionFile !defFile! archName !archName! > util\tmp.txt
		for /F %%i in (util\tmp.txt) do set id=%%i
		echo TaskID: !id!

		echo. 
		echo RESULTS:
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r out.ptptb > util\assmfile
		echo. 
		echo out file been writen in util\outfile

		exit /b
	)

	IF "%~1" == "assmraw" (
		Portable.RemoteTasks.Manager.exe -w -id -s AssembleRaw asmListing !asmListing! definitionFile !defFile! archName !archName! > util\tmp.txt
		for /F %%i in (util\tmp.txt) do set id=%%i
		echo TaskID: !id!

		echo. 
		echo RESULTS:
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r raw.out.bin > util\assmfileraw
		echo. 
		echo out file been writen in util\outfile

		exit /b
	)

	IF "%~1" == "diss" (
		Portable.RemoteTasks.Manager.exe -w -id -s Disassemble in.ptptb util\assmfile definitionFile !defFile! archName !archName! > util\tmp.txt
		
		for /F %%i in (util\tmp.txt) do set id=%%i
		echo TaskID: %id% 
		echo. 
		echo RESULTS:
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r disasmListing.txt  > util\dissfile		
		echo. 

		exit /b
	)

	IF "%~1" == "dissraw" (
		Portable.RemoteTasks.Manager.exe -w -id -s DisassembleRaw raw.in.bin util\assmfileraw definitionFile !defFile! archName !archName! > util\tmp.txt
		
		for /F %%i in (util\tmp.txt) do set id=%%i
		echo TaskID: %id% 
		echo. 
		echo RESULTS:
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r disasmListing.txt  > util\dissfileraw		
		echo. 

		exit /b
	)


	IF "%~1" == "valid" (
		Portable.RemoteTasks.Manager.exe -w -id -s VisualizeArchModel definitionFile !defFile! archName !archName! > util\tmp.txt
		
		for /F %%i in (util\tmp.txt) do set id=%%i
		echo TaskID: !id!
		echo. 
		echo RESULTS:
		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r syntaxTree.txt > util\syntaxTree
		Portable.RemoteTasks.Manager.exe -g !id! -r archInfoGraph.dgml > util\graphFile.dgml
		echo. 
		echo + syntaxTree and graph in util\

		exit /b
	)

	IF "%~1" == "run" (
		Portable.RemoteTasks.Manager.exe -w -id -s ExecuteBinaryWithInteractiveInput asmListing !asmListing! definitionFile !defFile! binaryFileToRun util/assmfile archName !archName! finishMnemonicName hlt codeRamBankName code ipRegStorageName PC stdinRegStName IN stdoutRegStName OUT > util\tmp.txt 
		for /F %%i in (util\tmp.txt) do set id=%%i

		Portable.RemoteTasks.Manager.exe -g !id!
		Portable.RemoteTasks.Manager.exe -g !id! -r stdout.txt > util\out.txt

		
		exit /b
	
	)


	echo invalid argument
	

) else ( echo gimmie arguments)