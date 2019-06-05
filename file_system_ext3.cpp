#include "inode.h"


//Exemplo de sistema de arquivos contendo 32 blocos de 4 bytes e 
//7 inodes. O diretório raiz é / e fica no inode 0.
// Não há outros arquivos ou diretórios no sistema de arquivo.
FILE* arquivo;
int main(){

	arquivo = fopen("PARTITION.bin", "rb+");
	init_with_root(arquivo);
	if(arquivo == NULL){
		init_with_root(arquivo);
	}

}

void init_with_root(FILE *file_system)
{

	file_system = fopen("PARTITION.bin", "wb+" );
	unsigned char numero_size_block = N_SIZE_BLOCKS;
	unsigned char numero_de_blocos = N_BLOCKS;
	unsigned char numero_de_inodes = N_INODES;


	fwrite(&numero_size_block, sizeof(numero_size_block), 1,file_system);
	fwrite(&numero_de_blocos, sizeof(numero_de_blocos), 1,file_system);
	fwrite(&numero_de_inodes, sizeof(numero_de_inodes), 1,file_system);


	cout<<sizeof(numero_size_block)<<endl;
	cout<<sizeof(numero_de_blocos)<<endl;
	cout<<sizeof(numero_de_inodes)<<endl;

	block_map[0] = 1;// primeiro é o root
	block_map[1] = 0;
	block_map[2] = 0;
	block_map[3] = 0;
	
	cout<<sizeof(block_map)<<endl;

	fwrite(&block_map, sizeof(block_map), 1,file_system);
	int i;

	inode tmp[N_INODES];
	
	for(i=0; i<N_INODES; i++)
	{
		tmp[i].is_used =0x00;
		tmp[i].is_dir=0x00;
		tmp[i].size=0x00;
		for(int j=0; j<3; j++){
			tmp[i].direct_blocks[j] = 0x00;
		}
		for(int j=0; j<3; j++){
			tmp[i].indirect_block[j] = 0x00;
		}
		for(int j=0; j<3; j++){
			tmp[i].double_indirect_blocks[j] = 0x00;
		}

	}
	fwrite(&tmp, sizeof(struct inode_)*N_INODES, 1 ,file_system);
	cout<<sizeof(tmp)<<endl;
	//fclose(file_system);

	//unsigned char index_root = 0;
	//fwrite(&index_root, sizeof(index_root), 1, file_system);
	//create_root(file_system);
	//int len = ftell(file_system);
	//cout<<len<<endl;
	
	//fwrite(&vet_block, sizeof(vet_block), 1, file_system);

	/*
	
	create_root(file_system);
	fseek_inode(file_system, 14);
	//fseek_inode(file_system, 13);
	int len = ftell(file_system);
	cout<<len<<endl;
	len = ftell(file_system);
	cout<<len<<endl;
	
	*/
}

void create_root(FILE * file_system){
	fclose(file_system);
	FILE * file = fopen("PARTITION.bin","r");
	fseek_inode(file_system,8);
	int len = ftell(file_system);
	cout<<len<<endl;

	inode tmp[N_INODES];
	fread(&tmp, sizeof(struct inode_)*N_INODES , 1 ,file);
	cout<<tmp[1].name<<endl;
	/* tmp.is_used = 1;
	tmp.is_dir  =1;
	tmp.size=0;
	strcpy(tmp.name,"/");
	 */
	//fwrite(&tmp, sizeof(tmp), 1, file_system);

	
	fwrite(&tmp, sizeof(inode), 1, file_system);

}

void insert_vet_block(FILE* file_system){
	//fseek_inode(file_system, 16);
	

	fwrite(&vet_block, sizeof(vet_block), 1, file_system);
	
	cout<<sizeof(vet_block)<<endl;

}

void fseek_inode(FILE* file_system, int number_inode)
{
	fseek(file_system, number_inode, SEEK_SET);
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

