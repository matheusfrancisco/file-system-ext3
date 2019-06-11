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


typedef struct inode_ inode;


#endif //INODE_H_

//hash f1
//3c59c9d17c2913fe514b46290de59d2abcb86242fec583ed09040465e8751812

//hash f2
//33e794b05434583d6a2ee485cd2a03d694e5570d0be3e87ad68bf909fd1d21ff