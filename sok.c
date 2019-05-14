#include<stdio.h>

int[][] get_map(int[][] m){

	FILE *ifp;
    if ((ifp = fopen("map","r")) == NULL)
		printf("Error : Cannot open the file.");
	else{
		while((c = getc(ifp)) != EOF)
			//m[][]
			;
		return m;
	}
	
	return m;
}

int main(void){

	return 0;
}
