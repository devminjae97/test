#include <stdio.h>
#include <termio.h>
char allmap[5][30][30]; // 전체 맵 배열
char arr[30][30]; // 이번 라운드 맵 배열
char undomap[5][30][30]; // 되돌리기 배열
char username[10]; // 유저 이름
int correctmap=1; // 잘못된맵 체크
int level=0; // 현재 라운드
int ng=0; // 새게임 체크
int rp=0; // 리플레이 체크
int ex=0; // 게임종료 체크
int cntmv=0; // 이동 횟수
int plx=0, ply=0; // 플레이어의 x,y좌표

int getch(); // getch함수 생성
void CheckRanking(); // 랭킹 파일이 없을 시 만드는 함수
void MakeMap(); // 전체 맵을 파일에서 받아오는 함수
void MakeArr(); // 맵을 arr로 받아오는 함수
void CheckArr(); // 잘못된 맵인지 체크하는 함수
void NowArr(); // 현재 맵상태 보여주기
void ClearArr(); // 배열 비우는 함수
int CheckClear(); // 맵 클리어 체크 함수
void FindCharacter(); // 캐릭터 위치를 찾는 함수
void Command(); // 명령어 실행 함수
void DisplayHelp(); // 명령어 목록을 보여주는 함수
int MoveCharacter(char c); // 캐릭터 이동 함수
void SaveArr(); // 맵 세이브 함수
void LoadArr(); // 맵 불러오기 함수
void SaveRanking(); // 랭킹 세이브 함수
void LoadRanking(); // 랭킹 불러오기 함수
void LoadAllRanking(); // 모든랭킹 불러오기 함수
void ClearUndo(); // 되돌리기 배열 비우는 함수
void SaveUndo(); // 되돌리기 맵 생성함수
int LoadUndo(); // 되돌린것 불러오는 함수

int getch(){ // getch함수 생성
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

int main() { // .==빈칸, @==캐릭터, #==벽, $==박스, O==박스를 채울곳
    MakeMap();
    CheckRanking();
    for (;level<5;level++){
        ClearArr();
        ClearUndo();
        MakeArr();
        CheckArr();
        if (correctmap==0)
            return 0;
        else
            printf("Start Map%d!\n", level+1);
        Command();
        if (ex) // 게임 종료 명령
            break;
        cntmv=0;
    }
    if (!ex)
        printf("\n모든맵을 클리어 하였습니다\n");
    return 0;
}

void Command() { // 명령어 실행 함수
    printf ("이름을 입력하세요: ");
    scanf("%s", &username);
    printf("Hello %s!\n", username);
    FindCharacter();
    printf("\n\n\n\n\n");
    NowArr();
    while (1){
        char cmd;
        cmd=getch();
        if (cmd=='h' || cmd=='j' || cmd=='k' || cmd=='l') // 이동 명령
            MoveCharacter(cmd);
        if (cmd=='n'){ // 처음부터 다시시작 명령
            level=-1;
            printf("처음부터 다시시작합니다.\n");
            break;
        }
        if (cmd=='r'){ // 이번 맵 다시시작 명령
            printf("이번 맵을 다시시작합니다.\n");
            MakeArr();
            NowArr();
        }
        if (cmd=='e'){ // 게임 종료 명령
            ex=1;
            printf("게임을 종료합니다.\n");
            break;
        }
        if (cmd=='x') {// 임시 클리어 명령어 (나중에 지울예정)
            SaveRanking();
            break;
        }
        if (cmd=='d') // 명령어 목록 보여주기 명령
            DisplayHelp();
        if (cmd=='s') // 맵 세이브 명령
            SaveArr();
        if (cmd=='f') // 맵 불러오기 명령
            LoadArr();
        if (cmd=='t'){ // 랭킹 불러오기 명령
            char R=0;
            R=getchar();
            if (R=='\n')
                LoadAllRanking();
            else if (R>='1' && R<='5')
                LoadRanking(R-'0');
            else
                printf("없는 레벨입니다\n");
        }
        if (cmd=='u') // 맵 되돌리기 명령
            LoadUndo();
        if (CheckClear()){// 맵 클리어 체크
            printf("Clear Map! %s!\n총 이동횟수는 %d번 입니다\n", username, cntmv);
            SaveRanking();
            break;
        }
        printf("\n\n\n\n\n");
    }
}
void CheckRanking(){ // 랭킹 파일이 없을 시 만드는 함수
    FILE *fp;
    if ((fp=fopen("ranking","r"))==NULL){
            fp=fopen("ranking","w");
            fprintf(fp, "999 999 999");
            fclose(fp);
    }
}

void MakeMap(){ // 전체 맵을 파일에서 받아오는 함수
    FILE *fp;
    fp=fopen("map", "r");
    fscanf(fp,"%s");
    for (int x=0;feof(fp)==0;x++){
        char newmap[30][30]; // 임시 맵 배열
        for (int i=0;i<30;i++){
            for (int j=0;j<30;j++){
                newmap[i][j]='X';
            }
        }
        for (int i=0;1;i++){
            char str[30]; // 한줄 입력받는 배열
            for (int j=0;j<30;j++){
                str[j]='X';
            }
            fscanf(fp,"%s",&str);
            if (str[0]!='.' && str[0]!='#' && str[0]!='$' && str[0]!='O' && str[0]!='@')
                break;
            else{
                for (int j=0;j<30;j++){
                    newmap[i][j]=str[j];
                }
            }
        }
        for (int i=0;i<30;i++){
            for (int j=0;j<30;j++){
                allmap[x][i][j]=newmap[i][j];
            }
        }
        printf("\n");
    }
    fclose(fp);
}

void MakeArr() { // 맵을 arr로 받아오는 함수
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]=allmap[level][i][j];
        }
    }
}

void CheckArr() { // 잘못된 맵인지 체크하는 함수
    int check1=0, check2=0;
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='O')
                check1++;
            if (arr[i][j]=='$')
                check2++;
        }
    }
    if (check1!=check2){
        printf("Map%d is Wrong Map!\n", level+1);
        correctmap=0;
    }
    else
        printf("Map%d is Correct Map!\n", level+1);
}

void FindCharacter() { // 캐릭터 위치 찾기
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='@'){
                plx=i;
                ply=j;
                break;
            }
        }
    }
}
void NowArr() { // 현재 맵상태 보여주기
    for (int i=0;i<30;i++){
        if (arr[i][0]=='X')
            break;
        for (int j=0;j<30;j++){
            if (arr[i][j]=='.')
                printf(" ");
            else if (arr[i][j]=='X')
                break;
            else
                printf("%c", arr[i][j]);
        }
        printf("\n");
    }
    printf ("\n\n이동횟수: %d\n", cntmv);
}

int CheckClear() { // 맵 클리어 체크 함수
    int chk=1;
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='O')
                chk=0;
        }
    }
    return chk;
}

void DisplayHelp(){ // 명령어 목록을 보여주는 함수
    printf("h(왼쪽), j(아래), k(위), l(오른쪽)\nu(undo)\nr(replay)\nn(new)\ne(exit)\ns(save)\nf(file load)\nd(display help)\nt(top)\n");
}

void ClearArr(){ // 배열 비우는 함수
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]='X';
        }
    }
}

int MoveCharacter(char c) { // 캐릭터 이동 함수
    cntmv++;
    SaveUndo();
    FindCharacter();
    int dx=0, dy=0;
    if (c=='h')
        dx=-1;
    if (c=='j')
        dy=-1;
    if (c=='k')
        dy=1;
    if (c=='l')
        dx=1;
    int x=plx+dx, y=ply+dy;
    if (arr[x][y]=='#'){ // 앞이 벽이면 종료
        NowArr();
        return 0;
    }
    if (arr[x][y]=='$'){ // 앞이 박스일때
        int ax=x+dx, ay=y+dy;
        if (arr[ax][ay]=='#' || arr[ax][ay]=='$'){ // 박스의 앞이 박스거나 벽이면 종료
            NowArr();
            return 0;
        }
        arr[ax][ay]='$';
        if (allmap[level][x][y]=='O') // 칸에 있던거면 빈칸 보이기
            arr[x][y]='O';
        else
            arr[x][y]='.';
    }
    arr[x][y]='@';
    if (allmap[level][plx][ply]=='O') // 칸에 있던거면 빈칸 보이기
            arr[plx][ply]='O';
    else
        arr[plx][ply]='.';
    NowArr();
    return 0;
}

void SaveArr(){ // 맵 세이브 함수
    FILE *fp;
    fp=fopen("save","w");
    fprintf(fp,"%d\n",cntmv);
    fprintf(fp,"%d\n",level);
    for (int i=0;i<30;i++){
        if (arr[i][0]=='X')
            break;
        fprintf(fp,"%s\n",arr[i]);
    }
    printf("레벨을 저장했습니다\n");
    fclose(fp);
}

void LoadArr(){ // 맵 불러오기 함수
    FILE *fp;
    if ((fp=fopen("save","r"))==NULL){
        printf("저장된 레벨이 없습니다\n");
    }
    else{
        ClearArr();
        ClearUndo();
        fscanf(fp,"%d\n",&cntmv);
        fscanf(fp,"%d\n",&level);
        for (int i=0;feof(fp)==0;i++)
            fscanf(fp,"%s\n",&arr[i]);
        printf("레벨을 불러왔습니다\n");
        NowArr();
    }
    fclose(fp);
}
void SaveRanking(){// 랭킹 세이브 함수
    FILE *fp, *dfp;
    fp=fopen("ranking","r");
    int cntrank=0;
    int dlevel=0, dcntmv=0;
    char dusername[10];
    while (1){ // 사이에 쓸때 앞에오는 랭킹 개수 세기
        fscanf(fp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        if (dlevel<level+1 || (dlevel==level+1 && dcntmv<=cntmv))
            cntrank++;
        else
            break;
    }
    fclose(fp);
    fp=fopen("ranking","r");
    dfp=fopen("newrank","w");
    for (int i=0;i<cntrank;i++){
        fscanf(fp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        fprintf(dfp, "%d %s %d\n", dlevel, dusername, dcntmv);
    }
    fprintf(dfp, "%d %s %d\n", level+1, username, cntmv);
    while (!feof(fp)){
        fscanf(fp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        if (dlevel!=999)
            fprintf(dfp, "%d %s %d\n", dlevel, dusername, dcntmv);
        else{
            fprintf(dfp, "999 999 999");
            break;
        }
    }
    fclose(fp);
    fclose(dfp);
    fp=fopen("ranking","w");
    dfp=fopen("newrank","r");
    while (!feof(dfp)){
        fscanf(dfp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        if (dlevel!=999)
            fprintf(fp, "%d %s %d\n", dlevel, dusername, dcntmv);
        else{
            fprintf(fp, "999 999 999");
            break;
        }
    }
    fclose(fp);
    fclose(dfp);
    remove("newrank");
}

void LoadAllRanking(){ // 모든랭킹 불러오기 함수
    int levelrank[6]={0}; // 레벨 확인
    int checkrank[6]={0}; // 레밸내 등수 확인
    FILE *fp;
    fp=fopen("ranking","r");
    for (int i=0;feof(fp)==0;){
        int dlevel=0, dcntmv=0;
        char dusername[10];
        fscanf(fp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        if (dlevel==i+1 && levelrank[i]==0){
            printf("\n---MAP %d RANKING---\nTOP  NAME       MOVE\n", i+1);
            levelrank[i]=1;
            i++;
        }
        if (dlevel<=5 && checkrank[i]<5){
            printf("%d.   %-10s %-5d\n", checkrank[i]+1, dusername, dcntmv);
            checkrank[i]++;
        }
    }
    fclose(fp);
}

void LoadRanking(int x){ // 랭킹 불러오기 함수
    int checkrank=0;
    printf("---MAP %d RANKING---\nTOP  NAME       MOVE\n", x);
    FILE *fp;
    fp=fopen("ranking","r");
    for (int i=0;feof(fp)==0;i++){
        int dlevel=0, dcntmv=0;
        char dusername[10];
        fscanf(fp, "%d %s %d", &dlevel, &dusername, &dcntmv);
        if (x==dlevel && checkrank<5){
            printf("%d.   %-10s %-5d\n", checkrank+1, dusername, dcntmv);
            checkrank++;
        }
    }
    fclose(fp);
}

void ClearUndo(){ // 되돌리기 배열 비우는 함수
    for (int i=0;i<5;i++){
        for (int j=0;j<30;j++){
            for (int k=0;k<30;k++){
                undomap[i][j][k]=='X';
            }
        }
    }
}

void SaveUndo(){ // 되돌리기 맵 생성함수
    for (int i=4;i>0;i--){
        for (int j=0;j<30;j++){
            for (int k=0;k<30;k++){
                undomap[i][j][k]=undomap[i-1][j][k];
            }
        }
    }
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            undomap[0][i][j]=arr[i][j];
        }
    }
}

int LoadUndo(){ // 되돌린것 불러오는 함수
    cntmv++;
    if (undomap[0][0][0]=='X'){
        printf("더이상 되돌릴수 없습니다.\n");
        return 0;
    }
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]=undomap[0][i][j];
        }
    }
    for (int i=0;i<4;i++){
        for (int j=0;j<30;j++){
            for (int k=0;k<30;k++){
                undomap[i][j][k]=undomap[i+1][j][k];
            }
        }
    }
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            undomap[4][i][j]='X';
        }
    }
    NowArr();
    return 0;
}