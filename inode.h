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

#define N_BLOCKS 32 // 32 blocos
#define N_SIZE_BLOCKS 4 // 4 bytes cada bloco
#define N_INODES 7 // 7 inodes
//#define N_BLOCKS 10



using namespace std;
 
 
struct inode_ {
    unsigned char is_used; //1
    unsigned char is_dir; //1
    char name[10]; // 8 
    char size; //1
    unsigned char direct_blocks[3]; //3
    unsigned char indirect_block[3];
    unsigned char double_indirect_blocks[3]; 
}; // 22bytes

unsigned char index_root = 0; //index root
unsigned char block_map[(int)ceil(N_BLOCKS/8.0)]; //mapa de bits

typedef struct inode_ inode;
char vet_block[N_BLOCKS*N_SIZE_BLOCKS]; 

void init_with_root(FILE * file_system);
int findFreeBlock(FILE * file_system);
void fseek_inode(FILE* file_system, int number_inode);

void create_root(FILE * file_system);
void insert_vet_block(FILE * file_system);
//void printInode(Inode i);
//void create_root(Inode root);
int find_inode_livre(FILE *file_system);
void create_file(FILE* file_system);
int find_block_free(FILE *file_system);

#endif //INODE_H_

//hash f1
//3c59c9d17c2913fe514b46290de59d2abcb86242fec583ed09040465e8751812

//hash f2
//33e794b05434583d6a2ee485cd2a03d694e5570d0be3e87ad68bf909fd1d21ff