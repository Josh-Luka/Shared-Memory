#ifndef SHARED_MEM_H
#define SHARED_MEM_H
#define _POSIX_C_SOURCE 200809L
#define SHM_NAME_LEN 256



typedef struct {
    int result;
    char word[4092];
} shared_mem_t;

#endif 