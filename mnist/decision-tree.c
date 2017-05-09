#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL 60000
#define WIDTH 28
#define HEIGHT 28

char images [HEIGHT][WIDTH][TOTAL];
char labels[TOTAL];

void endian_swap(unsigned int *x) {
  *x = (*x>>24)|((*x<<8)&0x00FF0000)|((*x>>8)&0x0000FF00)|(*x<<24);
}

void read_images(){
	FILE *file = fopen("./train-images-idx3-ubyte","rb");
	unsigned int magic_number;

	fread(&magic_number,4,1,file);
	endian_swap(&magic_number);

	if(magic_number!=2051){
		printf("%d ",magic_number);
		puts("\n");
		puts("Arquivo corrompido");
		exit(1);
	}

	unsigned int total_number;
	fread(&total_number,4,1,file);
	endian_swap(&total_number);

	if(total_number!=TOTAL){
		printf("%d ",total_number);
		puts("\n");
		puts("Arquivo corrompido");
		exit(1);
	}

	unsigned int rows,columns;
	fread(&rows,4,1,file);
	endian_swap(&rows);
	fread(&columns,4,1,file);
	endian_swap(&columns);
	if(rows!=HEIGHT || columns != WIDTH){
		printf("rows: %d, columns: %d",rows,columns);
		puts("\n");
		puts("Arquivo corrompido");
		exit(1);
	}

	for(int t=0 ; t<TOTAL ; t++){
		for(int i=0 ; i<rows ; i++){
			for(int j=0 ; j<rows ; j++){
				fread(images[i][j]+t,1,1,file);
			}
		}
	}
}

void read_labels(){
	FILE *file = fopen("./train-labels-idx1-ubyte","rb");
	unsigned int magic_number;

	fread(&magic_number,4,1,file);
	endian_swap(&magic_number);

	if(magic_number!=2049){
		printf("%d ",magic_number);
		puts("\n");
		puts("Arquivo corrompido");
		exit(1);
	}

	unsigned int total_number;
	fread(&total_number,4,1,file);
	endian_swap(&total_number);

	if(total_number!=TOTAL){
		printf("%d ",total_number);
		puts("\n");
		puts("Arquivo corrompido");
		exit(1);
	}

	for (int t=0;t<TOTAL;t++)
		fread(labels+t,1,1,file);
}
int main(){
	puts("reading labels");
	read_labels();
	puts("reading images");
	read_images();

	return 0;
}
