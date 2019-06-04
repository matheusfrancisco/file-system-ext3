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
	unsigned char tamanho_blocos = TAMANHO_BLOCO_DADOS;
	unsigned char numero_de_blocos = N_BLOCKS;
	unsigned char numero_de_inodes = N_INODES;


	fwrite(&tamanho_blocos, sizeof(unsigned char), 1,file_system);
	fwrite(&numero_de_blocos, sizeof(unsigned char), 1,file_system);
	fwrite(&numero_de_inodes, sizeof(unsigned char), 1,file_system);
	fwrite(&block_map, sizeof(unsigned char), 1,file_system);

	for(int i =0; i<N_INODES;i++){
		inode tmp;

		tmp.is_used = 0;
		tmp.is_dir  =0;
		tmp.size=0;
		//strcpy(tmp.name, "null");
		for(int j=0; j<TAMANHO_BLOCO_DADOS; j++){
			tmp.direct_blocks[i] = 0;
		}
		fwrite(&tmp, sizeof(tmp), 1, file_system);
	}
	fseek_inode_to_root(file_system, 0);
	inode tmp;
	tmp.is_used = 0;
	tmp.is_dir  =0;
	tmp.size=0;
	
	strcpy(tmp.name, "/");
	fwrite(&tmp, sizeof(tmp), 1, file_system);

}


void fseek_inode_to_root(FILE* file_system, int number_inode)
{
	fseek(file_system, 8, SEEK_SET);
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

