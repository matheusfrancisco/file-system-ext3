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
	//FILE *file_s = fopen("PARTITION.bin", "wb+" );	
	//create_root(arquivo);
	//create_file(arquivo);
	
	/**/
	/* block_map[0] = 7 ;
	block_map[1] =0;
	block_map[2] =0;
	block_map[3] =0;
	FILE *file_s = fopen("PARTITION_TESTE.bin", "wb+" );
	fwrite(&block_map, sizeof(block_map), 1, file_s);
	int l =find_block_free(file_s);
	cout<<l<<endl; */
}

int find_block_free(FILE *file_system)
{
	int number_block_free;

	unsigned char block_aux[(int)ceil(N_BLOCKS/8.0)];
	
	int i;
	fseek(file_system, 0, SEEK_SET);
	fread(&block_aux, sizeof(block_aux), 1, file_system);
 	int flag;
	for(int i = 0; i < N_BLOCKS; i++) {
		 flag = (block_aux[i/8] & (1 << (i%8)));
		 if(flag == 0)
		 	return i;
	}
	return -1;
}

void init_with_root(FILE *file_system)
{

	file_system = fopen("PARTITION.bin", "wb+" );

	unsigned char numero_size_block = N_SIZE_BLOCKS;
	unsigned char numero_de_blocos = N_BLOCKS;
	unsigned char numero_de_inodes = N_INODES;


	int le = ftell(file_system);
	cout<<le<<endl;
	fwrite(&numero_size_block, sizeof(numero_size_block), 1,file_system);
	le = ftell(file_system);
	cout<<le<<endl;
	fwrite(&numero_de_blocos, sizeof(numero_de_blocos), 1,file_system);
	fwrite(&numero_de_inodes, sizeof(numero_de_inodes), 1,file_system);

	le = ftell(file_system);
	cout<<le<<endl;
	cout<<sizeof(numero_size_block)<<endl;
	cout<<sizeof(numero_de_blocos)<<endl;
	cout<<sizeof(numero_de_inodes)<<endl;

	block_map[0] = 1;// primeiro é o root
	block_map[1] = 0;
	block_map[2] = 0;
	block_map[3] = 0;
	//mapa de bit 
	//20/8 -> 2 posicao do  vetor 
	//20%8 = 4

	//0x01 => 0b00000000
	//0x01<<3
	//0b0000100 -> fazer um and logico  result 00000000 bit livre..
	cout<<sizeof(block_map)<<endl;

	fwrite(&block_map, sizeof(block_map), 1,file_system);
	for(int index =0; index<N_INODES; index++){
		inode tmp;
		tmp.is_used =0;
		tmp.is_dir=0;
		tmp.size=0;
		for(int k=0; k<10;k++) tmp.name[k] = 0x0;
		for(int j=0; j<3; j++){
			memset(&tmp.direct_blocks[j],0x00,sizeof(tmp.direct_blocks[j]));
			memset(&tmp.indirect_block[j],0x00,sizeof(tmp.direct_blocks[j])); 	
			memset(&tmp.double_indirect_blocks[j],0x00,sizeof(tmp.double_indirect_blocks[j]));

		} 

		/* tmp.double_indirect_blocks[0] = 0;
		tmp.double_indirect_blocks[1] = 0;
		tmp.double_indirect_blocks[2] = 0; */
		fwrite(&tmp, sizeof(inode), 1 ,file_system);
		//free(&tmp);
	}

	//}
	//fclose(file_system);
	//fseek_inode(file_system,164);

	for(int index =0 ; index< 128; index++){
		vet_block[index] = 0;
	}

	fwrite(&vet_block, sizeof(vet_block), 1, file_system);

	//fwrite(&index_root, sizeof(index_root), 1, file_system);
	//create_root(file_system);
	//int len = ftell(file_system);
	//cout<<len<<endl;
	/* for(int index =0 ; index< 128; index++){
		vet_block[index] = 0;
	}

	fwrite(&vet_block, sizeof(vet_block), 1, file_system);
 */
	/*
	
	create_root(file_system);
	fseek_inode(file_system, 14);
	//fseek_inode(file_system, 13);
	int len = ftell(file_system);
	cout<<len<<endl;
	len = ftell(file_system);
	cout<<len<<endl;
	
	*/
	create_root(arquivo);

	//fclose(file_system);
}

void create_file(FILE* file_system){
	//file_system = fopen("PARTITION.bin", "wb+" );
	int position = find_inode_livre(file_system);
	cout<<position<<endl;

}

int find_inode_livre(FILE *file_system){
	fseek_inode(file_system, 8);
	
	for(int index=0;index<N_INODES; index++)
	{
		inode tmp;
		fread(&tmp, sizeof(struct inode_), 1, file_system);
		cout<<index<<endl;
		cout<<tmp.is_used<<endl;
		//cout<<tmp[index].is_used<<cout;
		if(tmp.is_used == 0x0) return index;
	}
}

void create_root(FILE * file_system){
//	file_system = fopen("PARTITION.bin", "wb+");

	fseek_inode(file_system,7);
	int len = ftell(file_system);
	cout<<len<<endl;

	inode tmp;
	//fread(&tmp, sizeof(struct inode_), 1 ,file_system);
	//fseek_inode(file_system,8);
	tmp.is_dir =1;
	tmp.is_used=1;
	strcpy(tmp.name,"/");
	for(int k=1; k<9;k++)tmp.name[k] =0x0;
	tmp.size=0; 
	cout<<tmp.name<<endl;
	for(int j=0; j<3; j++){
			memset(&tmp.direct_blocks[j],0x00,sizeof(tmp.direct_blocks[j]));
			memset(&tmp.indirect_block[j],0x00,sizeof(tmp.direct_blocks[j])); 	
			memset(&tmp.double_indirect_blocks[j],0x00,sizeof(tmp.double_indirect_blocks[j]));

	} 
	fseek_inode(file_system,7);

	fwrite(&tmp, sizeof(inode), 1, file_system);
	
	/* tmp.is_used = 1;
	tmp.is_dir  =1;
	tmp.size=0;
	strcpy(tmp.name,"/");
	 */
	//fwrite(&tmp, sizeof(tmp), 1, file_system);


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

