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

#define N_BLOCKS 32
#define N_SIZE_BLOCKS 4
#define N_INODES 7
//#define N_BLOCKS 10

#define INODE_SIZE sizeof(inode)



#define TAMANHO_INODE 


using namespace std;
 
 
struct inode_ {
    unsigned char is_used;
    unsigned char is_dir;
    char name[10];
    unsigned char size;
    unsigned char direct_blocks[N_BLOCKS];
    //unsigned char indirect_block[1];
    //unsigned char double_direct_blocks[1];
};

unsigned char block_map[4];
typedef struct inode_ inode;
unsigned char vet_block[N_BLOCKS*N_SIZE_BLOCKS] ={0};

void init_with_root(FILE * file_system);
int findFreeBlock(FILE * file_system);
void fseek_inode(FILE* file_system, int number_inode);

void create_root(FILE * file_system);
//void printInode(Inode i);
//void create_root(Inode root);

#endif //INODE_H_

//hash f1
//3c59c9d17c2913fe514b46290de59d2abcb86242fec583ed09040465e8751812

//hash f2
//33e794b05434583d6a2ee485cd2a03d694e5570d0be3e87ad68bf909fd1d21ff