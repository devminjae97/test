#include<stdio.h>
#include<stdlib.h>
#include<termio.h>

 
int x=0, y=0 isOnGame = 0;
int map[156][31] = {0};

void get_map(void);
void set_map(void);
void mv(int ch);
int ingame(int);

int getch(void);
 

int main(void){
 

	/*

	 *  Sokoban

	 *	@:Sokoban(Player), #:Wall, $:Gold, O:Safe, .:Blank

	*/

 

	char player_name[11] = {0};

	int level = 1;

	system("clear");

	printf("Start...\n");

	// Load maps

	if(get_map() == 1)
		goto final;

	// Get a name

	printf("input name : ");

	scanf("%10s", player_name);

	printf("Hello %s\n", player_name);

	

	set_map(level, player_name);

	final :
	
	printf(" System >> Shutting down this Programme.\n");
	
	return 0;
}


int get_map(void){

	FILE *ifp;

	int c, rtrnValue = 1, i = 0, j = 0, cmp = 0;	

    if ((ifp = fopen("map","r")) == NULL){
		printf("Error : Cannot open the map file!\n");
		goto end;
	}
	else{
		while((c = getc(ifp)) != EOF){				// '1' = 49, '\n' = 10

			if(c == '\n'){
				map[i++][j] = c;
				j = 0;
			}
			else if(c == 'e'){
				map[i][j] = c;
				break;
			}
			else{                               // Check maps
				if(c == '$')
					cmp++;
				else if(c == 'O')
					cmp--;
				else if(c >= '1' && c<= '5')
					if(cmp != 0){
						printf("Error : The number of $ and O is differnt!\n");
						goto end;
					}
				map[i][j++] = c;
			}
		}
	}
	rtrnValue = 0;
	
	end : 
	
	return rtrnValue;
}
 
void set_map(int lvl, char n[11]){
	
	int i, j = 0;
	static contmp_map[31][31] = {0};
	
	system("clear");
	
	for(i=0; map[i][0] != lvl + 48; i++);
	if(isOnGame == 0)
		for(int ii = 0; (map[i][j] < '1' || map[i][j] > '5') && map[i][j] != 'e'; i++)
			for(j = 0; j < 31){
				contmp_map[ii][j] = map[i][j];
				isOnGame = 1;
			}
	
	for(ii = 0; ii< 31; ii++){ //수정 요 for문 추가
		for(j = 0; j <31; j++){
			switch(contmp_map[ii][j]){
				case '@' : putchar('@'); x = j; y = ii; break;
				case '#' : putchar('#'); break;
				case '$' : putchar('$'); break;
				case 'O' : putchar('O'); break;
				case '.' : putchar(' '); break;
				case '\n' : putchar('\n');ii++; j = -1; break;
				default : break;
			}
		}
	}
	
	


	printf("(Command) ");

	return;
}

int getch(void){
	int ch;
	
	struct termios buf;
	struct termios save;
	
	tcgetattr(0, &save);
	buf = save;
	
	buf.c_lflag&=~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	
	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	
	return ch;
}

void mv(int ch){
	int tmp_x = x, tmp_y = y;
	
	
	switch(ch){
		case 'h' : tmp_x--; break;
		case 'j' : tmp_y--; break;
		case 'k' : tmp_y++; break;
		case 'l' : tmp_x++; break;
	}
	if(map[tmp_y][tmp_x] == '#')
		break;
	else if (map[tmp_y][tmp_x] == '.' || map[tmp_y][tmp_x] == 'O'){
		map[y][x] = '.';
		map[tmp_y][tmp_x] = '@';
		break;
	}
	else if (map[tmp_y][tmp_x] == '$'){
		mv(ch);
		continue;
	}
	default : break;	
}

int ingame(int lvl){
	int steps = 0;
	
	
	return steps;
}




