#ifndef INODE_H_
#define INODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h> 
#include <bits/stdc++.h>

#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#define TAMANHO_BLOCO_DADOS 32
#define TAMANHO_BLOCO_INDICES 12
#define N_INODES 2
#define N_BLOCKS 10
#define INODE_SIZE sizeof(inode)
#define INODE_REGION N_BLOCKS
#define BLOCKS_REGION N_BLOCKS + (N_INODES * INODE_SIZE)
using namespace std;
 
typedef uint8_t  data_block[TAMANHO_BLOCO_DADOS];
typedef uint16_t  bloco_indice[TAMANHO_BLOCO_INDICES];

struct inode_ {
    uint8_t status;
    uint8_t dir;
    char name[8];
    data_block direct_blocks;
    uint8_t tam;
};



// verificar em qual bloco é o livre
unsigned char block_manager[N_BLOCKS] = { 0 };



typedef struct inode_ inode;


void init(FILE * file_system);
int findFreeBlock(FILE * file_system);
void fseek_inode(FILE* file_system, int number_inode);


//void printInode(Inode i);
//void create_root(Inode root);

#endif //INODE_H_