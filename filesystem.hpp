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
#include <bitset>

using namespace std;

class FileSystem
{
    private:
        unsigned char number_blocks;
        unsigned char number_inodes;
        unsigned char sizeof_blocks;
        string root_name;
        string file_system_name_create;

        string name_dir_create;
        //int num_inodes;
        
        /*bit map informations*/
        int bitMapSize;
        unsigned char bitmap_size;
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

        //infos
    
    public:
        void init(FILE * partition, int n_blocks, int n_inodes, int size_blocks);
        void create_file(string name_file_system);
        void create_bit_map(FILE *partition);
        void create_vector_inodes(FILE *partition);
        void create_vector_blocks(FILE * partition);
        void insert_index_root(FILE *partition);

        void change_root_direct_block(FILE* partition , unsigned char address);
        
        /*methods*/
        void change_bit_map(FILE* partition, int number_block_free);
        void create_root(string root_name_, FILE* partition);
        int find_block_free(FILE* partition);
        int find_inode_free(FILE *partition);
        void add_file_root(string file_name,string conteudo, FILE* partition);
        void insert_information_in_block(FILE* partition, int position, string info, int number_blocks_used);
        void change_bit_map_file(FILE* partition, int number_block_free, int number_block_used);
        void change_size_root(FILE * partition);
        

        void command_inputs();
        void addFile();
        void addDir(FILE * partition, string name_dir);
        
        //auxiliar functions

        void get_file_system_informations(FILE* partition);

        void convert_string_to_char();

        static vector<string> split(string str, char delimiter);
        
        
};
#endif