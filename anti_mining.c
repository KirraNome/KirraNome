#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define MAX_CMD_LENGTH 256
#define THRESHOLD_CPU_USAGE 100.0
#define MONITOR_INTERVAL 5 // seconds
#define LOG_FILE "/var/log/anti_mining.log"

void log_message(const char *message) {
    FILE *log_fp = fopen(LOG_FILE, "a");
    if (log_fp == NULL) {
        perror("Failed to open log file");
        return;
    }
    time_t now = time(NULL);
    fprintf(log_fp, "[%s] %s\n", ctime(&now), message);
    fclose(log_fp);
}

void kill_process(pid_t pid) {
    if (kill(pid, SIGKILL) == 0) {
        char message[MAX_CMD_LENGTH];
        snprintf(message, sizeof(message), "Process %d killed due to high CPU usage", pid);
        log_message(message);
    } else {
        perror("Failed to kill process");
    }
}

double get_cpu_usage(pid_t pid) {
    char path[MAX_CMD_LENGTH];
    double cpu_usage = 0.0;

    snprintf(path, sizeof(path), "ps -p %d -o %%cpu --no-headers", pid);
    FILE *fp = popen(path, "r");
    if (fp == NULL) {
        perror("Failed to execute ps command");
        return -1.0;
    }
    fscanf(fp, "%lf", &cpu_usage);
    pclose(fp);

    return cpu_usage;
}

int main() {
    log_message("Anti-mining program started");

    while (1) {
        FILE *fp = popen("ps -eo pid,pcpu --no-headers", "r");
        if (fp == NULL) {
            perror("Failed to execute ps command");
            log_message("Failed to execute ps command");
            exit(EXIT_FAILURE);
        }

        pid_t pid;
        double cpu_usage;
        while (fscanf(fp, "%d %lf", &pid, &cpu_usage) != EOF) {
            if (cpu_usage > THRESHOLD_CPU_USAGE) {
                kill_process(pid);
            }
        }

        pclose(fp);
        sleep(MONITOR_INTERVAL);
    }

    log_message("Anti-mining program terminated");
    return 0;
}