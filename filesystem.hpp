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
#include <openssl/sha.h>
#include <openssl/bio.h>

using namespace std;

class FileSystem
{
    private:
        unsigned char number_blocks;
        unsigned char number_inodes;
        unsigned char sizeof_blocks;
        string root_name;
        string file_system_name_create;
        string name_file_to_add;
        string info_to_insert_file;
        string name_dir_create;
        //unsigned char block_free_in_root;
        //int valor_block_add_direct_dir;
        //int num_inodes;
        int next_end_block_to_root;

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

        void change_root_direct_block(FILE* partition);
        void check_root_direct_block(FILE* partition);
        /*methods*/
        void change_bit_map(FILE* partition, int number_block_free);
        void create_root(string root_name_, FILE* partition);
        int find_block_free(FILE* partition);
        int find_inode_free(FILE *partition);
        void add_file_root(string file_name,string conteudo, FILE* partition);
        void insert_information_in_block(FILE* partition, int position, string info, int number_blocks_used);
        void change_bit_map_adddir(FILE* partition, int number_block_free, int number_block_used);
        void change_size_root(FILE * partition);
        void change_bit_map_addfile(FILE * partition, int number_block_free, int number_need_to_insert);

        void command_inputs();
        void addFile(FILE* partition, string file_name, string info_file);
        void addDir(FILE * partition, string name_dir);
        void add_number_inode_dir_in_vet_block(FILE* partition, int inode_index);
        unsigned char get_address_free_block_in_root(FILE * partition);

        //auxiliar functions
        void change_inode_direct(FILE* partition,int number_inode,  int frist_address, int numbers_used);
        void get_file_system_informations(FILE* partition);

        void convert_string_to_char();
        void printSha256(string path);

        static vector<string> split(string str, char delimiter);
        
        
};
#endif