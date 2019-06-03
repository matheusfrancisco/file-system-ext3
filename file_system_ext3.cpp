#include "inode.h"


//Exemplo de sistema de arquivos contendo 32 blocos de 4 bytes e 
//7 inodes. O diretório raiz é / e fica no inode 0.
// Não há outros arquivos ou diretórios no sistema de arquivo.
FILE* arquivo;
int main(){

	arquivo = fopen("PARTITION.bin", "rb+");

	init(arquivo);
	if(arquivo == NULL){
		init(arquivo);
	}
}

void init(FILE *file_system)
{

	file_system = fopen("PARTITION.bin", "wb+" );

	//fwrite(&block_manager, sizeof(block_manager), 1, file_system);


	for(int inode_n= 0; inode_n< N_INODES; inode_n++){
		inode tmp ;

		tmp.status = 0;
		tmp.dir  =0;
		tmp.tam=0;
		strcpy(tmp.name, "null");

	}
	for (int block;block<N_BLOCKS; block++)
	{
		fwrite(&block_manager, sizeof(block_manager), 1, file_system);
	}

	int inode_root = 0;
	char * name_root = "/";

	inode novo;
	inode pai;
	
	strcpy(novo.name, name_root);
	novo.dir = 1;
	novo.status =1;
	novo.tam = 0;
	novo.data_block = findFreeBlock(file_system);
	fseek_inode(file_system, inode_root);
	fwrite(&novo, sizeof(novo), 1, file_system);

}

void fseek_inode(FILE* file_system, int number_inode)
{
	fseek(file_system, INODE_REGION + (number_inode*INODE_SIZE), SEEK_SET);
}

int findFreeBlock(FILE * file_system) {
	unsigned char block_aux[N_BLOCKS];
	int i;
	fseek(file_system, 0, SEEK_SET);
	fread(&block_aux, sizeof(block_aux), 1, file_system);

	for(int i = 0; i < N_BLOCKS; i++) {
		if(block_aux[i] == 0) {
			block_aux[i] = 1;
			fseek(file_system, 0, SEEK_SET);
			fwrite(&block_aux, sizeof(block_aux), 1, file_system);
			return i;
		}
	}
	return -1;
}
/*
void create_root(Inode root){

     //Inode myInode;
     root.flag = 1;
     root.type = 1;
     root.number = 0;
     root.father_inode = 0; //INODE DO DIRETORIO PAI
     strncpy(root.name, "/", sizeof(Inode::name));
}


void printInode(Inode i){
    printf("Flag: %d\n", i.flag);
    printf("Type: %d\n", i.type);
    printf("Number: %d\n", i.number);
    printf("Father: %d\n", i.father_inode);
    printf("Name: %s\n", i.name);

    for(int j = 0; j < 7; j++){
        printf("Block[%d]: %d\n", j, i.blocks[j]);
    }
    cout<<endl;
}

*/
