#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

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
#include "inode.h"
#include <string>
#include <vector>
#include <cstring>

using namespace std;

class FileSystem
{
    private:
        unsigned char number_blocks;
        unsigned char number_inodes;
        unsigned char sizeof_blocks;
        string root_name;
        //int num_inodes;
        
        /*bit map informations*/
        int bitMapSize;
        int bit_map_start;
        int bit_map_end;
        /*inodes address*/
        int start_inode_map;
        int finish_inode_map;
        /**/
        int root_dir_index_in_file;
        unsigned char root_inode = 0;
        int relative_index_root;
        /*vector block map*/
        int vector_block_size;
        int vector_block_start_in_map;
        int vector_block_end_in_map;

        FILE *file_system_name;
        //inode _inode ;

    
    public:
        void init();
        void create_file(FILE * partition);
        void create_bit_map(FILE *partition);
        void create_vector_inodes(FILE *partition);
        void create_vector_blocks(FILE* partition);
        void insert_index_root(FILE *partition);

        /*methods*/
        void create_root(string root_name_, FILE* partition);
        //auxiliar functions
        static vector<string> split(string str, char delimiter);
        
        
};
#endif