#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TOTAL 60000
#define WIDTH 28
#define HEIGHT 28
#define log2(x) log(x)/log(2)

unsigned char images [HEIGHT][WIDTH][TOTAL];
unsigned char labels[TOTAL];

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
			for(int j=0 ; j<columns ; j++){
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

float entropy(int count[10]){
	int total = 0;
	for(int t=0;t<10;t++)
		total+=count[t];

	float probabilities[10];
	for(int t=0;t<10;t++)
		probabilities[t] = count[t]/(float)total;

	//sum information to achieve entropy
	float sum = 0;
	for(int t=0;t<10;t++)
		sum-= probabilities[t]/log2(probabilities[t]);

	return sum;
}

float information_gain(int left_count[10],int right_count[10]){
	int total[10]={};
	int left_sum = 0;
	int right_sum = 0;
	for(int t=0;t<10;t++){
		total[t] = left_count[t]+right_count[t];
		left_sum+=left_count[t];
		right_sum+=right_count[t];
	}

	float parent_entropy = entropy(total);
	float left_entropy = entropy(left_count);
	float right_entropy = entropy(right_count);

	float new_entropy = (left_sum*left_entropy+right_sum*right_entropy)/(float)(left_sum+right_sum);

	return new_entropy-parent_entropy;
}

void train(int total){
	unsigned char thresh = 0x88;
	int left_count[10]  = {};
	int right_count[10]  = {};

	int x,y;
	float max_gain = 0;
	int better_left[10];
	int better_right[10];
	for(int i=0 ; i<HEIGHT ; i++){
		for(int j=0 ; j<WIDTH ; j++){
			for(int t=0;t<total;t++){
				if(images[i][j][t] > thresh){
					left_count[(int)labels[t]]++;
				}else{
					right_count[(int)labels[t]]++;
				}

				float gain = information_gain(left_count,right_count);
				if (gain > max_gain){
					for(int t=0;t<10;t++){
						better_left[t] = left_count[t];
						better_right[t] = right_count[t];
					}
					x=i;y=j;
					max_gain = gain;
				}
			}

		}
	}

	for(int t=0;t<10;t++)
		printf("%d: %d,%d\n",t,better_left[t],better_right[t]);
	printf("(%d , %d) -> %f\n",x,y,max_gain);
}

int main(){
	read_labels();
	read_images();

	train(1000);

	return 0;
}
