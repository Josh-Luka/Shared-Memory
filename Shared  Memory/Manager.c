#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHM_NAME_LEN 256

typedef struct {
    int result;
    char word[4092];
} shared_mem_t;

int main(int count, char *args[]) {
    if (count < 2) {
        printf("Please enter at least one word.\n");
        return 1;
    }

    int total = count - 1;
    char *words[total];

    for (int i = 0; i < total; i++) {
        words[i] = args[i + 1];
    }

    pid_t pids[total];
    char memnames[total][SHM_NAME_LEN];

    for (int i = 0; i < total; i++) {
        int pipes[2];

        if (pipe(pipes) != 0) {
            perror("Pipe didn't work");
            return 1;
        }

        sprintf(memnames[i], "/shm%d%d", getpid(), i);
        int mem = shm_open(memnames[i], O_CREAT | O_RDWR, 0666);

        if (mem < 0) {
            perror("Couldn't open shared memory");
            return 1;
        }

        ftruncate(mem, sizeof(shared_mem_t));
        shared_mem_t *shared = mmap(NULL, sizeof(shared_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);

        if (shared == MAP_FAILED) {
            perror("Mapping memory failed");
            return 1;
        }

        shared->result = -1;
        strncpy(shared->word, words[i], sizeof(shared->word));
        shared->word[sizeof(shared->word) - 1] = '\0';
        munmap(shared, sizeof(shared_mem_t));
        close(mem);

        pid_t child = fork();

        if (child == 0) {
            close(pipes[1]);
            char num[10];
            sprintf(num, "%d", pipes[0]);
            execl("./Palindrome", "Palindrome", num, NULL);
            perror("Manager: exec failed");
            exit(1);
        } else {
            pids[i] = child;
            close(pipes[0]);
            printf("Manager: forked process with ID %d.\n", child);
            write(pipes[1], memnames[i], strlen(memnames[i]) + 1);
            printf("Manager: wrote shm name to pipe (%ld bytes)\n", strlen(memnames[i]) + 1);
            close(pipes[1]);
        }
    }

    for (int i = 0; i < total; i++) {
        printf("Manager: waiting on child process ID %d...\n", pids[i]);
        waitpid(pids[i], NULL, 0);

        int mem = shm_open(memnames[i], O_RDONLY, 0666);
        if (mem < 0) {
            perror("Opening shared memory failed");
            continue;
        }

        shared_mem_t *shared = mmap(NULL, sizeof(shared_mem_t), PROT_READ, MAP_SHARED, mem, 0);
        if (shared == MAP_FAILED) {
            perror("Mapping memory failed");
            close(mem);
            continue;
        }

        if (shared->result == 1) {
            printf("Manager: result 1 read from shared memory. \"%s\" is a palindrome.\n", shared->word);
        } else {
            printf("Manager: result 0 read from shared memory. \"%s\" is not a palindrome.\n", shared->word);
        }

        munmap(shared, sizeof(shared_mem_t));
        close(mem);
        shm_unlink(memnames[i]);
    }

    printf("Manager: exiting.\n");
    return 0;
}