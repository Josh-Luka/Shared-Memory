#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_NAME_LEN 256

typedef struct {
    int result;
    char word[4092];
} shared_mem_t;

int checkPalindrome(char *text) {
    int start = 0;
    int end = strlen(text) - 1;
    while (start < end) {
        if (text[start] != text[end]) {
            return 0;
        }
        start++;
        end--;
    }
    return 1;
}

int main(int count, char *args[]) {
    if (count != 2) {
        fprintf(stderr, "Palindrome: incorrect usage\n");
        return 1;
    }

    int pipeRead = atoi(args[1]);
    char memName[SHM_NAME_LEN];

    if (read(pipeRead, memName, SHM_NAME_LEN) <= 0) {
        perror("Palindrome: pipe read failed");
        return 1;
    }

    printf("Palindrome process [%d]: starting.\n", getpid());

    int mem = shm_open(memName, O_RDWR, 0666);
    if (mem < 0) {
        perror("Palindrome: shm_open failed");
        return 1;
    }

    shared_mem_t *shared = mmap(NULL, sizeof(shared_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);
    if (shared == MAP_FAILED) {
        perror("Palindrome: mmap failed");
        return 1;
    }

    printf("Palindrome process [%d]: read word \"%s\" from shared memory\n", getpid(), shared->word);

    int result = checkPalindrome(shared->word);

    if (result == 1) {
        printf("Palindrome process [%d]: %s *IS* a palindrome\n", getpid(), shared->word);
    } else {
        printf("Palindrome process [%d]: %s *IS NOT* a palindrome\n", getpid(), shared->word);
    }

    shared->result = result;
    printf("Palindrome process [%d]: wrote result (%d) to shared memory.\n", getpid(), result);

    munmap(shared, sizeof(shared_mem_t));
    close(mem);
    close(pipeRead);
    return 0;
}