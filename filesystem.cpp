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

void FileSystem::create_file(string name_file_system){
    //cout<<"File::"<<name_file_system<<"create"<<endl;
    char *name_file_bin;
    name_file_bin  = (char*)malloc(sizeof(char)*name_file_system.length());
        
    for(int i=0;i<name_file_system.length();i++ ){
        name_file_bin[i] = name_file_system[i] ;
    }
    file_system_name = fopen(name_file_bin, "wb+");
    if(file_system_name == NULL)
    {
        file_system_name = fopen(name_file_bin, "wb+");

    }

}

void FileSystem::create_bit_map(FILE *partition){
    unsigned char bit_map[(int)ceil(number_blocks/8.0)];
    bitMapSize = (int)ceil(number_blocks/8.0);
    
    for(int i=0; i<(int)ceil(number_blocks/8.0); i++ )
    {
        bit_map[i] = 0;
    }
    
    if(partition == NULL){
       
    }
    fseek(partition, 3, SEEK_SET);
    
    //start bit map
    bit_map_start = 3;
    fwrite(&bit_map, sizeof(bit_map), 1,partition);
    //end bit map
    bit_map_end = ftell(partition); 
    start_inode_map = ftell(partition);   
}

void FileSystem::create_vector_inodes(FILE *partition){
    fseek(partition, bit_map_end, SEEK_SET);
    
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
    
}

void FileSystem::create_vector_blocks(FILE *partition ){
   
    fseek(partition, root_dir_index_in_file,SEEK_SET);
    vector_block_start_in_map = ftell(partition);
    //cout<<"Vector blocks start here: "<<"||"<<vector_block_start_in_map<<" ||"<<endl;
    unsigned char vector_block[number_blocks*sizeof_blocks];

    for(int i=0; i<number_blocks*sizeof_blocks; i++ )
    {
        vector_block[i] = 0;
        //cout<<bit_map[i]<<endl;
    }

    //vector_block_start_in_map = ftell(partition);
    fwrite(&vector_block, sizeof(vector_block), 1,partition);
    vector_block_end_in_map = ftell(partition);
    //cout<<"Vector blocks finish here: "<<"||"<<vector_block_end_in_map<<" ||"<<endl;

}

void FileSystem::create_root(string root_name_, FILE* partition){
    
    //unsigned char name_[root_name_.size() + 1];
    fseek(partition, start_inode_map, SEEK_SET);
    int im_here = ftell(partition);

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
    //cout<<number_block_free_<<endl;
    change_bit_map(file_system_name, number_block_free_);
}

void FileSystem::insert_index_root(FILE * partition){
    //cout<<"Inode vector start here: ||"<<finish_inode_map<<"||"<<endl;
    fseek(partition, finish_inode_map, SEEK_SET);
    fwrite(&root_inode, sizeof(unsigned char), 1, partition);
    root_dir_index_in_file= ftell(partition);
    
}

void FileSystem::change_bit_map(FILE* partition, int number_block_free){

    int position_in_map = (int)floor(number_block_free/8.0);
    unsigned char bit_map_aux[(int)ceil(number_blocks/8.0)];

    fseek(partition, bit_map_start, SEEK_SET);
    fread(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
    
    int current_value_in_map =  bit_map_aux[(number_block_free/8)];
    int new_value = (1<< (current_value_in_map %8));
    //cout<<"SET BIT"<<new_value<<endl;
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
    //cout<<"Position to start map inode : "<<start_inode_map<<endl;
    //cout<<"Position to end map inode : "<<finish_inode_map<<endl;
    //cout<<"Position to root dir : "<<root_dir_index_in_file<<endl;
    //cout<<"Position to vet inode start : "<<vector_block_start_in_map<<endl;

    //cout<<"Position to insert informations in block: "<<position<<endl;
    
    int real_position = position + vector_block_start_in_map;
    fseek(partition, real_position, SEEK_SET);
    
    //cout<<"number block used : "<<number_block_used<<endl;

    unsigned char block[sizeof_blocks*number_block_used];

    fread(&block,sizeof(sizeof_blocks) * sizeof_blocks * number_block_used,1, partition );
    for(int i=0; i<(sizeof_blocks*number_block_used); i++)
    {
        block[i] = info[i];
    }
    fseek(partition, real_position, SEEK_SET);
    fwrite(&block, sizeof(block), 1, partition);

}

void FileSystem::change_bit_map_adddir(FILE* partition, int number_block_free, int number_block_used)
{
    //cout<<"Number block free: "<<number_block_free<<endl;
    int position_in_map = (int)floor(number_block_free/8.0);
    unsigned char bit_map_aux[(int)ceil(number_blocks/8.0)];
    //cout<<"Position in bitMap: "<<position_in_map<<endl;

    fseek(partition, bit_map_start, SEEK_SET);
    fread(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
    
    unsigned char new_value;
    unsigned char  current_value_in_map = bit_map_aux[position_in_map];
    new_value = current_value_in_map | (current_value_in_map<<1);

    int value_novo = new_value;

    //cout<<"New value in bitMap: "<<value_novo<<endl;
    bit_map_aux[position_in_map] = new_value;
    
    fseek(partition, bit_map_start, SEEK_SET);
    fwrite(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
}

void FileSystem::change_size_root(FILE * partition)
{
    unsigned char *file;
    //cout<<bit_map_end<<"aqui"<<endl;
    fseek(partition, start_inode_map+12, SEEK_SET);
    unsigned char size_ ;
    //inode root_[number_inodes];

    fread(&size_, sizeof(size_) ,1,  partition);
    
    //root_[0].size = size_ + 1;
    fseek(partition, start_inode_map+12, SEEK_SET);
    size_ +=1;
    //int position = start_inode_map + 12;
    //fseek(partition, start_inode_map, SEEK_SET);
   fwrite(&size_, sizeof(size_), 1,partition);
    
}
void FileSystem::change_bit_map_addfile(FILE * partition, int number_block_free, int number_need_to_insert)
{
    int position_in_map;
    unsigned char bit_map_aux[(int)ceil(number_blocks/8.0)];
    //cout<<"Position in bitMap: "<<position_in_map<<endl;

    fseek(partition, bit_map_start, SEEK_SET);
    fread(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
    
    unsigned char new_value;
    unsigned char  current_value_in_map ;
    int test;
    int test2;
    int carry = number_block_free;
    for(int i = 0; i< number_need_to_insert; i++){
        position_in_map = (int)floor(number_block_free/8.0);

        current_value_in_map = bit_map_aux[position_in_map];
        test = bit_map_aux[position_in_map];
        //cout<<"old valor"<<test<<endl;
        new_value = current_value_in_map | (current_value_in_map<<1);
        test2 = new_value;
        //cout<<"novo valor: "<<test2<<endl;
        bit_map_aux[position_in_map] = new_value;
        
        number_block_free++;
   
    }
   
    //new_value = current_value_in_map | (current_value_in_map<<1);

    //int value_novo = new_value;

    //cout<<"New value in bitMap: "<<value_novo<<endl;
   
    fseek(partition, bit_map_start, SEEK_SET);
    fwrite(&bit_map_aux, sizeof(bit_map_aux), 1, partition);
}


void FileSystem::add_file_root(string file_name,string conteudo, FILE* partition){

    //find inode free
    int inode_free = find_inode_free(partition);
    
    uint8_t position_in_vet_inode = start_inode_map + inode_free* sizeof(inode);
    //cout<<inode_free<<"|index inode free|"<<endl;

    int tamanho_string = conteudo.length();
    int number_blocks_to_use = (int)ceil(tamanho_string/(float)sizeof_blocks);

    int number_frist_free_block = find_block_free(partition);

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
    fseek(partition, position_in_vet_inode, SEEK_SET);
    fwrite(&_file, sizeof(_file), 1, partition);
    
   
    int position_block_in_map =  number_frist_free_block * sizeof_blocks;
    
    //cout<<"Frist block free: "<<number_frist_free_block<<endl;
    //cout<<"Number block used: "<<number_blocks_to_use<<endl;
    //change_root_direct_block(partition, inode_free);
    change_size_root(partition);

    change_bit_map_addfile(partition, number_frist_free_block, number_blocks_to_use);
    add_number_inode_dir_in_vet_block(partition, inode_free);
    insert_information_in_block(partition, position_block_in_map, conteudo , number_blocks_to_use);
    change_inode_direct(partition,inode_free,number_frist_free_block , number_blocks_to_use);


    
}

void FileSystem::change_inode_direct(FILE * partition, int number_inode, int frist_address, int numbers_used){
    int address_inode = number_inode*sizeof(inode) + start_inode_map;


    fseek(partition, address_inode, SEEK_SET);
    inode inode_;
    fread(&inode_, sizeof(inode_), 1, partition);
    for(int index=0; index<3; index++){
        if(inode_.direct_blocks[index] == 0x00){

            if(numbers_used ==3){
                inode_.direct_blocks[index] = frist_address+index;

            }else{
                if(index == 2){
                    inode_.direct_blocks[index] =0x0;
                }else{
                    inode_.direct_blocks[index] = frist_address+index;
                }
                

            }
        }
    }
    fseek(partition, address_inode, SEEK_SET);
    fwrite(&inode_, sizeof(inode_), 1, partition);
                    
}


void FileSystem::init(FILE *partition, int n_blocks, int n_inodes, int size_blocks){
    //cout<<"System file name : "<<"|| "<<name_file_system<<" ||"<<endl;
    //cout<<"Sizeof blocks : "<<"|| "<<size_blocks<<" ||"<<endl;
    //cout<<"Number blocks system : "<<"|| "<<n_blocks<<" ||"<<endl;
    //cout<<"Number inode in system : "<<"|| "<<n_inodes<<" ||"<<endl;

    //file_system_name_create = name_file_system;
    sizeof_blocks= size_blocks;
    number_blocks = n_blocks;
    number_inodes = n_inodes; 

  
	fwrite(&sizeof_blocks, sizeof(unsigned char), 1,partition);
	fwrite(&number_blocks, sizeof(unsigned char), 1,partition);
	fwrite(&number_inodes, sizeof(unsigned char), 1,partition);
    //insert sizeof block, number_block, number_inodes.
    create_bit_map(partition);
    create_vector_inodes(partition);
    insert_index_root(partition);
    create_vector_blocks(partition);
    string rootName = "/";
    create_root(rootName, partition);
   

    
}
void FileSystem::get_file_system_informations(FILE *partition){
    unsigned char informations_file_system[3];
    fread(&informations_file_system, sizeof(unsigned char)*3 ,1,  partition);
   
    /* int get_size_blocks = informations_file_system[0];
    int get_number_blocks = informations_file_system[1];
    int get_number_inodes = informations_file_system[2]; */
    
    number_blocks =informations_file_system[1];
    number_inodes = informations_file_system[2];
    sizeof_blocks = informations_file_system[0];
    bitmap_size = (int)ceil(number_blocks/8.0);

    fseek(partition, 3, SEEK_SET);
    bit_map_start = ftell(partition);
    fseek(partition, bit_map_start+bitmap_size, SEEK_SET);
    bit_map_end = ftell(partition) -1;
    start_inode_map = ftell(partition);
    //start_inode_map = start_inode_map+1;
    fseek(partition, start_inode_map + (number_inodes)*sizeof(inode), SEEK_SET );
    finish_inode_map = ftell(partition)-1;
    root_dir_index_in_file = ftell(partition);
    fseek(partition, root_dir_index_in_file+1, SEEK_SET);
    vector_block_start_in_map = ftell(partition);


  /*   cout<<bit_map_start<<endl;
    cout<<bit_map_end<<endl;
    cout<<start_inode_map<<endl;
    cout<<finish_inode_map<<endl;
    cout<<root_dir_index_in_file<<endl;

    cout<<vector_block_start_in_map<<endl;
 */

}
unsigned char  FileSystem::get_address_free_block_in_root(FILE * partition){
    unsigned char root_block[sizeof_blocks];
    fseek(partition,vector_block_start_in_map, SEEK_SET);
    fread(&root_block, sizeof(root_block), 1, partition);
    for(int address=0;address< sizeof_blocks;address++){
        if(root_block[address] == 0x00){
            //cout<<address<<"AQUIII"<<endl;
            return address;
            //break;
        }
    }
    /* fseek(partition,vector_block_start_in_map, SEEK_SET);
    fwrite(&root_block, sizeof(root_block), 1, partition ); */

}

void FileSystem::change_root_direct_block(FILE* partition){

    unsigned char address = get_address_free_block_in_root(partition);
    inode tmp;
    fseek(partition, start_inode_map, SEEK_SET);
    fread(&tmp, sizeof(inode), 1, partition);
    int test = address;
    for(int i=0;i<3;i++){
        if(tmp.direct_blocks[i] == 0x0){
            //cout<<"Foi add no block direto "<<test<<endl;
            tmp.direct_blocks[i] =address;
            break;
        }
    }
    fseek(partition, start_inode_map, SEEK_SET);
    fwrite(&tmp, sizeof(tmp), 1, partition );

    

}

void FileSystem::addFile(FILE* partition, string file_name, string info_file){
    get_file_system_informations(partition);
    add_file_root(file_name, info_file,  partition);
}

void FileSystem::add_number_inode_dir_in_vet_block(FILE * partition,  int inode_index){
    
    unsigned char root_block[sizeof_blocks];
    //cout<<"inicio vetor de block "<<vector_block_start_in_map<<endl;
   
        fseek(partition,vector_block_start_in_map, SEEK_SET);
        fread(&root_block, sizeof(root_block), 1, partition);
        int last_index;
        for(int i=0;i<sizeof_blocks; i++){
            if(root_block[i] == 0x00){
                //cout<<"Numero do inode add no dir "<<inode_index<<endl;
                last_index = i;
                root_block[i] = inode_index;
                break;
            }
            last_index = i;
        }
       
            fseek(partition,vector_block_start_in_map, SEEK_SET);
            fwrite(&root_block, sizeof(root_block), 1, partition );
       
}

void FileSystem::check_root_direct_block(FILE *partition){

    unsigned char root_block[sizeof_blocks];
    int last_index;
    fseek(partition, vector_block_start_in_map,  SEEK_SET);
    fread(&root_block, sizeof(root_block), 1, partition);
    for(int i=0; i<sizeof_blocks; i++){
        if(root_block[i] != 0x00){
            last_index = i;
        }
    
    }
    if(last_index == sizeof_blocks -1){
        next_end_block_to_root = find_block_free(partition);
    }
}

void FileSystem::addDir(FILE* partition, string name_dir){

    get_file_system_informations(partition);
    //check_root_direct_block(partition);
    int number_inode_free = find_inode_free(partition);
    int next_block_free = find_block_free(partition);

    if(number_inode_free == -1) {
        cout<<"Doesn't exit free Inodes"<<endl;
        exit(0);
    }
    if(next_block_free == -1){
        cout<<"Doesn't exit free blocks"<<endl;
        exit(0);
    }

    char name_dir_to_create[name_dir.length()];
    for( int i=0; i<name_dir.length(); i++){
        name_dir_to_create[i] = name_dir[i];
    }

    fseek(partition,start_inode_map + (number_inode_free)*sizeof(inode), SEEK_SET );

    inode new_dir;
    new_dir.is_used = 1;
    new_dir.is_dir = 1;
    for(int n=0;n<10;n++)
        new_dir.name[n] = 0;
    for(int s=0; s<strlen(name_dir_to_create); s++){
        new_dir.name[s] = name_dir_to_create[s];
    }
    new_dir.size =0;
    for(int j= 0; j<3; j++){
            memset(&new_dir.direct_blocks[j],0x00,sizeof(new_dir.direct_blocks[j]));
			memset(&new_dir.indirect_block[j],0x00,sizeof(new_dir.direct_blocks[j])); 	
			memset(&new_dir.double_indirect_blocks[j],0x00,sizeof(new_dir.double_indirect_blocks[j]));
    }
    fwrite(&new_dir, sizeof(inode), 1, partition);
    
    add_number_inode_dir_in_vet_block(partition, number_inode_free);

    change_size_root(partition);
    //change_root_direct_block(partition);
    
    //cout<<"This is the next block free in vector blocks"<<next_block_free<<endl;
    change_bit_map_adddir(partition, next_block_free, 1);

}

void FileSystem::command_inputs(){

   
        string sentence; 
        const char delimiter =' ';
        getline(cin,sentence);
        vector<string> sep =   split(sentence, delimiter);
        
        int len_vector = sep.size();
        if(len_vector == 3){
            name_dir_create = sep[2];
        }
        if(len_vector == 4){
            name_file_to_add = sep[2];
            info_to_insert_file = sep[3];

        }

        if(len_vector <= 2){
            cout<<"This is not a command valid"<<endl;
            cout<<"You should type: "<<endl;
            cout<<"init FileSystemName.bin SIZE_BLOCK NUMBER BLOCKS NUMBER INODE"<<endl;
            cout<<"addFile FileSystemName.bin nameFile.txt data"<<endl;
            cout<<"addDIr FileSystemName.bin DirName"<<endl;
            exit(0);
        }
        file_system_name_create = sep[1];

        int len = 0;
        char name_file_bin[file_system_name_create.length()];

        for(int i=0;i<file_system_name_create.length();i++ ){
            name_file_bin[i] = file_system_name_create[i] ;
            len++;
        } 
        if (name_file_bin[strlen(name_file_bin)-1] == 10)
		    name_file_bin[strlen(name_file_bin)-1] = 0x00;
        //cout<<name_file_bin<<endl;
        //create_file(file_system_name_create);
        file_system_name = fopen(name_file_bin, "rb+");
        if(file_system_name == NULL){
            file_system_name = fopen(name_file_bin, "rb+");
        }


        //create file again 
        //create_file(file_system_name_create);

        if(sep[0].compare("init") == 0){
            init(file_system_name,atoi(sep[3].c_str()), atoi(sep[4].c_str()), atoi(sep[2].c_str()));
            exit(0);
        }else if(sep[0].compare("addDir") == 0){
            addDir(file_system_name, name_dir_create);
        }else if(sep[0].compare("add") == 0){
            addFile(file_system_name, name_file_to_add, info_to_insert_file);
        }else if(sep[0].compare("exit")){
            return ;
        }else{
            cout<<"Command not found"<<endl;
        } 

   
}