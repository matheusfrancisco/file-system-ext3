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
    
    //cout<<sizeof(bit_map)<<endl;
    fseek(partition, 3, SEEK_SET);
    
    //start bit map
    bit_map_start = 3;
    fwrite(&bit_map, sizeof(bit_map), 1,partition);
    //end bit map
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
    //vector_block_start_in_map = ftell(partition);
    fwrite(&vector_block, sizeof(vector_block), 1,partition);
    //vector_block_end_in_map = ftell(partition);
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
    int number_block_free_ = find_block_free(file_system_name);
    cout<<number_block_free_<<endl;
    change_bit_map(file_system_name, number_block_free_);
}

void FileSystem::insert_index_root(FILE * partition){
    fseek(partition, finish_inode_map, SEEK_SET);
    fwrite(&root_inode, sizeof(unsigned char), 1, partition);
    root_dir_index_in_file= ftell(partition);
}

void FileSystem::change_bit_map(FILE* partition, int number_block_free){

    int position_in_map = (int)ceil(number_block_free/8.0);
    unsigned char bit_map_aux[(int)ceil(number_blocks/8.0)];
    fseek(partition, bit_map_start, SEEK_SET);
    fread(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
    int current_value_in_map =  bit_map_aux[(number_block_free/8)];
    //cout<<current_value_in_map<<endl;
    int new_value = (1<< (current_value_in_map%8));
    //cout<<new_value<<endl;
    //bit_map_aux[number_block_free/8] |= (1<<(number_block_free%8));
    //int new_value_bit  = (current_value<<1);
   //cout<<position_in_map<<endl;
    bit_map_aux[position_in_map] = new_value;
    fseek(partition, bit_map_start, SEEK_SET);
    //int t = ftell(partition);
    //cout<<t<<"aqui"<<endl;
    //cout<<sizeof(bit_map_aux)<<endl;
    fwrite(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
}

int FileSystem::find_block_free(FILE *partition)
{
	int number_block_free;

	unsigned char block_aux[(int)ceil(number_blocks/8.0)];
	
	int i;
	fseek(partition, bit_map_start, SEEK_SET);
	fread(&block_aux, sizeof(block_aux), 1, partition);
 	int flag;
	for(int i = 0; i < number_blocks; i++) {
		 flag = (block_aux[i/8] & (1 << (i%8)));
		 if(flag == 0)
		 	return i;
	}
	return -1;
}

int FileSystem::find_inode_free(FILE *partition){
    fseek(partition, start_inode_map, SEEK_SET);
    inode vet_inode[number_inodes];
    fread(&vet_inode, sizeof(inode)*number_inodes, 1, partition);
    for(int index=0;index<number_inodes; index++){
        if(vet_inode[index].is_used == 0){
            return index;
        }
    }
    return -1;
}
void FileSystem::insert_information_in_block(FILE* partition, int position, string info, int number_block_used){
    int real_possition_in_file = position + root_dir_index_in_file;
    cout<<"Real postition in vector block"<<"|"<<real_possition_in_file<<endl;

    fseek(partition, real_possition_in_file, SEEK_SET);
    
    cout<<"number block used"<<"|"<<number_block_used<<endl;

    unsigned char block[sizeof_blocks*number_block_used];
    fread(&block,sizeof(sizeof_blocks) * sizeof_blocks * number_block_used,1, partition );
    for(int i=0; i<(sizeof_blocks*number_block_used); i++)
    {
        block[i] = info[i];
    }

    fwrite(&block, sizeof(block), 1, partition);

}

void FileSystem::change_bit_map_file(FILE* partition, int number_block_free, int number_block_used)
{
    int position_in_map = (int)ceil(number_block_free/8.0);
    unsigned char bit_map_aux[(int)ceil(number_blocks/8.0)];
    
    fseek(partition, bit_map_start, SEEK_SET);
    fread(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
    
    int new_value;
    int current_value_in_map;
    while(number_block_free< number_block_used){
        current_value_in_map =  bit_map_aux[(number_block_free/8)];
        new_value = (1<< (current_value_in_map%8));

        number_block_free +=1;
        bit_map_aux[(number_block_free/8)] = new_value;
    }

    cout<<"Olha ele aqui"<<new_value<<endl;
    bit_map_aux[position_in_map] = new_value;
    
    fseek(partition, bit_map_start, SEEK_SET);
    fwrite(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
}

void FileSystem::change_size_root(FILE * partition)
{
    fseek(partition, start_inode_map, SEEK_SET);
    unsigned char old_size;
    
    inode root_;

    fread(&root_, sizeof(root_),1,  partition);
    old_size = root_.size;
    old_size +=1;
    int position = start_inode_map + 12;
    fseek(partition, position, SEEK_SET);
    fwrite(&old_size, sizeof(unsigned char), 1,partition);

}

void FileSystem::add_file_root(string file_name,string conteudo, FILE* partition){

    //find inode free
    int inode_free = find_inode_free(partition);
    uint8_t position_in_vet_inode = inode_free* sizeof(inode);
    cout<<inode_free<<"|index inode free|"<<endl;
    
    fseek(partition, position_in_vet_inode, SEEK_SET);
    inode _file;

    _file.is_dir=0;
    _file.is_used=1;
    _file.size=conteudo.length();
    for(int n=0;n<10;n++)
        _file.name[n] = 0;
    for(int s=0; s<file_name.length(); s++){
        _file.name[s] = file_name[s];
    }
    for(int j= 0; j<3; j++){
            memset(&_file.direct_blocks[j],0x00,sizeof(_file.direct_blocks[j]));
			memset(&_file.indirect_block[j],0x00,sizeof(_file.direct_blocks[j])); 	
			memset(&_file.double_indirect_blocks[j],0x00,sizeof(_file.double_indirect_blocks[j]));
    }
    fwrite(&_file, sizeof(_file), 1, partition);
    
    int tamanho_string = conteudo.length();
    int number_blocks_to_use = (int)ceil(tamanho_string/(float)sizeof_blocks);

    int number_block_free_ = find_block_free(partition);
    //cout<<number_block_free_<<endl;
    int position_block_in_map = number_block_free_ * sizeof_blocks;
    
    insert_information_in_block(partition, position_block_in_map, conteudo , number_blocks_to_use);

    change_bit_map_file(partition, number_block_free_, number_blocks_to_use);
    
    change_size_root(partition);



    //create inode
    //find block free
    //insert data in bitmap
    //change bit_map
    //add position block  in inode
    
}

void FileSystem::init(){
    string sentence; 
    const char delimiter =' ';
 
    string command;
    
    // get string init fs.bin 4 32 7 
    getline(cin,sentence);
    
    // split string and return 
    // sep = [init, fs.bin, 4, 32, 7]

    vector<string> sep =   split(sentence, delimiter);
    

    command = sep[0];
    char *name_file_bin;
    name_file_bin  = (char*)malloc(sizeof(char)*sep[1].length());

    for(int i=0;i<sep[1].length();i++ )
        name_file_bin[i] = sep[1][i] ;

    sizeof_blocks= atoi(sep[2].c_str());
    number_blocks = atoi(sep[3].c_str());
    number_inodes = atoi(sep[4].c_str()); 

    //create file
    create_file(file_system_name);
    if(file_system_name == NULL){
        create_file(file_system_name);
    }
    //open file
    file_system_name = fopen(name_file_bin, "wb+");
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

    string s= "/test.txt";
    string c= "123456";

    add_file_root(s, c, file_system_name);

    
}