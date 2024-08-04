⚜️ Anti-miner for ubuntu.
This is an anti-miner written in c, it terminates processes that load the processor under 100%

fast install:
• apt update
• apt install gcc
• gcc -o anti_mining anti_mining.c
to create log file:
• sudo touch /var/log/anti_mining.log
• sudo chmod 666 /var/log/anti_mining.log
to start:
screen(enter)
sudo ./anti_mining
