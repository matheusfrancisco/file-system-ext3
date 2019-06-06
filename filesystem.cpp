#include "filesystem.hpp"
#include "inode.h"


vector<string> FileSystem::split(string str, char delimiter) {
 
 vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
 
  return internal;
}

void FileSystem::create_file(FILE *partition){
    cout<<"File::"<<"create fs.bin"<<endl;
    partition = fopen("fs.bin", "wb+");

}

void FileSystem::create_bit_map(FILE *partition){
    
    unsigned char bit_map[(int)ceil(number_blocks/8.0)];
    bitMapSize = (int)ceil(number_blocks/8.0);

    for(int i=0; i<(int)ceil(number_blocks/8.0); i++ )
    {
        bit_map[i] = 0;
        //cout<<bit_map[i]<<endl;
    }
    
    cout<<sizeof(bit_map)<<endl;
    fseek(partition, 3, SEEK_SET);
    bit_map_start = 3;
    fwrite(&bit_map, sizeof(bit_map), 1,partition);
    bit_map_end = ftell(partition);    
}

void FileSystem::create_vector_inodes(FILE *partition){
    fseek(partition, bit_map_end, SEEK_SET);
    cout<<bit_map_end<<"|bit_map_end|"<<endl;
    relative_index_root = ftell(partition);
    start_inode_map = ftell(partition);
    for(int index=0; index<number_inodes; index++){
        inode tmp;
        tmp.is_used =0x0;
        tmp.is_dir =0x0;
        for(int n=0; n<10;n++){
            tmp.name[n] = 0x0;
        }
        tmp.size =0x0;
        for(int j= 0; j<3; j++){
            memset(&tmp.direct_blocks[j],0x00,sizeof(tmp.direct_blocks[j]));
			memset(&tmp.indirect_block[j],0x00,sizeof(tmp.direct_blocks[j])); 	
			memset(&tmp.double_indirect_blocks[j],0x00,sizeof(tmp.double_indirect_blocks[j]));
        }
        fwrite(&tmp, sizeof(tmp), 1,partition);
    }
    finish_inode_map =ftell(partition);
    cout<<finish_inode_map<<"|bit_map_end|"<<endl;

}

void FileSystem::create_vector_blocks(FILE* partition){
    unsigned char vector_block[number_blocks*sizeof_blocks];
    
    for(int i=0; i<number_blocks*sizeof_blocks; i++ )
    {
        vector_block[i] = 0;
        //cout<<bit_map[i]<<endl;
    }
    fseek(partition, root_dir_index_in_file,SEEK_SET);

    fwrite(&vector_block, sizeof(vector_block), 1,partition);

}

void FileSystem::create_root(string root_name_, FILE* partition){
    
    //unsigned char name_[root_name_.size() + 1];
    fseek(partition, start_inode_map, SEEK_SET);
    int im_here = ftell(partition);
    cout<<"I'm here:"<<im_here<<endl;

    inode _root;

    _root.is_dir=1;
    _root.is_used=1;
    _root.size=0;
    for(int n=0;n<10;n++)
        _root.name[n] = 0;
    for(int s=0; s<root_name_.length(); s++){
        _root.name[s] = root_name_[s];
    }
    for(int j= 0; j<3; j++){
            memset(&_root.direct_blocks[j],0x00,sizeof(_root.direct_blocks[j]));
			memset(&_root.indirect_block[j],0x00,sizeof(_root.direct_blocks[j])); 	
			memset(&_root.double_indirect_blocks[j],0x00,sizeof(_root.double_indirect_blocks[j]));
    }
    fwrite(&_root, sizeof(_root), 1, partition);

}

void FileSystem::insert_index_root(FILE * partition){
    fseek(partition, finish_inode_map, SEEK_SET);
    fwrite(&root_inode, sizeof(unsigned char), 1, partition);
    root_dir_index_in_file= ftell(partition);
}

void FileSystem::init(){
    string sentence; 
    const char delimiter =' ';
 
    string command;
    string name_file_bin;
    
    // get string init fs.bin 4 32 7 
    getline(cin,sentence);
    
    // split string and return 
    // sep = [init, fs.bin, 4, 32, 7]

    vector<string> sep =   split(sentence, delimiter);
    

    command = sep[0];
    name_file_bin = sep[1];
    
    sizeof_blocks= atoi(sep[2].c_str());
    number_blocks = atoi(sep[3].c_str());
    number_inodes = atoi(sep[4].c_str()); 

    //create file
    create_file(file_system_name);
    if(file_system_name == NULL){
        create_file(file_system_name);
    }
    //open file
    file_system_name = fopen("fs.bin", "wb+");
	fwrite(&sizeof_blocks, sizeof(unsigned char), 1,file_system_name);
	fwrite(&number_blocks, sizeof(unsigned char), 1,file_system_name);
	fwrite(&number_inodes, sizeof(unsigned char), 1,file_system_name);
    //insert sizeof block, number_block, number_inodes.
   
    create_bit_map(file_system_name);
    create_vector_inodes(file_system_name);
    insert_index_root(file_system_name);
    create_vector_blocks(file_system_name);

    cout<<"Insira o nome do root"<<endl;

    cin>>root_name;
    cout<<root_name<<endl;
    create_root(root_name, file_system_name);

    
}