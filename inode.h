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

#define N_INODES 7
#define N_BLOCKS 10

#define INODE_SIZE sizeof(inode)

#define INODE_REGION N_BLOCKS
#define BLOCKS_REGION N_BLOCKS + (N_INODES * INODE_SIZE)

#define TAMANHO_INODE 


using namespace std;
 
 
struct inode_ {
    unsigned char is_used;
    unsigned char is_dir;
    char name[10];
    unsigned char size;
    unsigned char direct_blocks[TAMANHO_BLOCO_DADOS];
};

unsigned char block_map[4] = {0};


typedef struct inode_ inode;


void init(FILE * file_system);
int findFreeBlock(FILE * file_system);
void fseek_inode_to_root(FILE* file_system, int number_inode);


//void printInode(Inode i);
//void create_root(Inode root);

#endif //INODE_H_
