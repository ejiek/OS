@ECHO OFF
ECHO Script is started
set N=1000
FOR /l %%i IN (1, 1, %N%) DO (
	START "TCP client %%i" "tcp_stress.exe"
)
ECHO Script is finished
ECHO Number of process with image "client.exe"
Exit