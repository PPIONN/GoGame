//8*8 그리드 바둑

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

#define UP  72         // 위쪽 이동 (방향키로 이동)
#define DOWN 80         // 아래쪽 이동 (방향키로 이동)
#define LEFT 75         // 왼쪽 이동 (방향키로 이동)
#define RIGHT 77      // 오른쪽 이동 (방향키로 이동)
#define ENTER 32      // 바둑돌 놓기 (엔터)
#define RETURN 8        // 바둑돌 무르기 (백스페이스)

#define CLEAR 0         // 배열 초기화
#define NONE 0
#define MAPSIZE 8      // 바둑판 크기 지정 (8*8)
#define START 1
#define END 0
#define NO 0
#define player1 1      // 사용자 1
#define player2 2      // 사용자 2
#define PLAY 1         // 바둑 시작할 때
#define OK 1
#define YES 1
#define TRUE 1
#define FALSE 0
#define OK_COUNT 4

int map[MAPSIZE][MAPSIZE];                  // 출력용 Map
int checkHouse[MAPSIZE][MAPSIZE];           // 놓는 위치를 기준으로 집이 있는지 판단하기 위한 Map
int x, y = 3;
int displayX = 0, displayLineY = 10, displayY = 12;   // 커서 출력
char mapGrid[] = "┌┬┐├┼┤└┴┘";            // 바둑판 그리기

int countReset1 = 0;                     // for player 1) 집이 생겨 돌을 치울 때 지우기 위한 count 수
int countReset2 = 0;                     // for player 2) 집이 생겨 돌을 치울 때 지우기 위한 count 수
int count_return = 0;                     //두 번까지 무르기 확인하는 변수
int totalCount = 0;                      // 지금까지 놓은 바둑돌의 개수

int STARTFLAG = 0;                        // 바둑 집 찾는 loop 반복 제어
int ret_count1 = 0;                       // player1 두 수 무르기
int ret_count2 = 0;                       // player2 두 수 무르기
int play_count = 0;


void ClearStones();                     // 바둑돌 배열 초기화 함수
void ClearMap();                        // 바둑판 초기화 함수

void InputSelect(int player);                  // 방향 및 바둑돌 착수
void ShowCheckerBoard();                        // 바둑판 상황 출력 함수
void ShowCursorPos(int x, int y);                  // 바둑판 배열에 보이는 현재 위치를 커서로 보여 주는 함수

int DeleteCheck();                           // 놓은 바둑돌 주위에 집이 있는지 여부를 count로 최종 확인하는 함수
int DecisionFlag(int x, int y);               // 놓는 위치가 유효한지 판단할 때 사용
void RemoveStone(int x, int y, int player_number);   // 바둑돌이 잡혔을때 처리
void CheckMyStonePos(int x, int y, int player_number);   // 바둑돌을 놓을 경우 현재 위치가 상대방 돌을 포위하는 위치인지 확인
int CheckPlayerHousePos(int x, int y, int player_number);   // 현재 두는 위치가 상대방 집인지 확인
int FindHouse(int x, int y, int player_number);
int Find(int player_number);
int GameStatus();                           // 현재 게임 상태를 확인, 게임을 계속 진행할지를 판단


void ClearStones() {               // 바둑돌 값 배열 초기화 함수

    int i, j;

    for (i = 0; i < MAPSIZE; i++) {
        for (j = 0; j < MAPSIZE; j++) {
            checkHouse[i][j] = CLEAR;      // checkHouse 초기화
        }
    }
}

void ClearMap() {      // 바둑판 초기화 함수

    int x, y;

    for (y = 0; y < MAPSIZE; y++) {
        for (x = 0; x < MAPSIZE; x++) {
            map[y][x] = CLEAR;
        }
    }
}

int DecisionFlag(int x, int y) {
    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE)   // 맵의 위치에 벗어난 경우의 Flag 판단
        return TRUE;

    if (checkHouse[y][x] == OK)               // checkHouse가 1인 경우
        return TRUE;

    return FALSE;
}

int DeleteCheck(int count) {
    return count == OK_COUNT;
}

int CheckPlayerHousePos(int x, int y, int player_number) { // 내가 두는 곳이 상대방의 집인지 여부를 네 방향 판단

    int result;
    int count;

    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE)    // 맵의 위치를 벗어난 곳에 놓으려 할 때

        return OK;

    if (map[y][x] == player_number)            // 이미 자신이 놓은 위치에 놓으려고 할 때
        return OK;

    if (map[y][x] == NONE)                     // 바둑돌이 없는 곳에 놓으려고 하는 경우
        return NO;

    count = 0;
    result = NO;
    checkHouse[y][x] = OK;

    if (DecisionFlag(x, y - 1) == FALSE) {     // y - 1이 현재 돌이 있는가

        if (CheckPlayerHousePos(x, y - 1, player_number)) {   // 어떤 플레이어의 돌로 쌓여 있는지 확인 --> 재귀

            count = count + 1;

        }
    }
    else { count = count + 1; }

    if (DecisionFlag(x - 1, y) == FALSE) {          // x - 1이 현재 돌이 있는가

        if (CheckPlayerHousePos(x - 1, y, player_number)) {     // 어떤 플레이어의 돌로 쌓여 있는지 확인 --> 재귀
            count = count + 1;
        }
    }
    else { count = count + 1; }

    if (DecisionFlag(x + 1, y) == FALSE) {

        if (CheckPlayerHousePos(x + 1, y, player_number)) { count = count + 1; }
    }
    else { count = count + 1; }

    if (DecisionFlag(x, y + 1) == FALSE) {
        if (CheckPlayerHousePos(x, y + 1, player_number)) { count = count + 1; }
    }
    else { count = count + 1; }

    if (count == OK_COUNT) {            // 놓은 위치가 집에 둘러 쌓여있음 or 집이 되는 경우 판단
        result = OK;
    }

    return result;

}

int FindHouse(int x, int y, int player_number) {

    int result;
    int count;

    // 맵의 위치를 벗어난 곳에 놓으려 할 때
    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE)  return OK;

    if (map[y][x] == player_number)  return OK;          // 이미 자신이 놓은 위치에 놓으려고 할 때

    count = 0;
    result = NO;
    checkHouse[y][x] = OK;

    if (DecisionFlag(x, y - 1) == FALSE) {           // y - 1이 현재 돌이 있는가
        if (CheckPlayerHousePos(x, y - 1, player_number)) {      // 어떤 플레이어의 돌로 쌓여 있는지 확인 --> 재귀
            count = count + 1;
        }
    }
    else { count = count + 1; }

    if (DecisionFlag(x - 1, y) == FALSE) {           // x - 1이 현재 돌이 있는가

        if (CheckPlayerHousePos(x - 1, y, player_number)) {      // 어떤 플레이어의 돌려 쌓여 있는지 확인 --> 재귀
            count = count + 1;
        }
    }
    else { count = count + 1; }

    if (DecisionFlag(x + 1, y) == FALSE) {

        if (CheckPlayerHousePos(x + 1, y, player_number)) { count = count + 1; }
    }
    else { count = count + 1; }

    if (DecisionFlag(x, y + 1) == FALSE) {

        if (CheckPlayerHousePos(x, y + 1, player_number)) { count = count + 1; }
    }
    else { count = count + 1; }

    if (count == OK_COUNT) {           // 놓은 위치가 집에 둘러 쌓여있거나, 집이 되는 경우 판단
        result = OK;
    }
    return result;
}

void RemoveStone(int x, int y, int player_number) // 집이 생겨 점수를 내고 돌을 지울 때 --> 재귀로 동작
{
    if (x < 0 || x >= MAPSIZE || y < 0 || y >= MAPSIZE) return;

    if (map[y][x] == player_number) return;

    if (map[y][x] == NONE) return;

    map[y][x] = CLEAR;

    if (player_number == player1) countReset2++;

    if (player_number == player2) countReset1++;

    if (x + 1 < MAPSIZE) RemoveStone(x + 1, y, player_number);

    if (x - 1 >= 0) RemoveStone(x - 1, y, player_number);

    if (y + 1 < MAPSIZE) RemoveStone(x, y + 1, player_number);

    if (y - 1 >= 0) RemoveStone(x, y - 1, player_number);
}

void InputSelect(int player) {            // 현재 바둑을 두고 있는 플레이어의 좌표 이동
    char check;
    int player_number;
    int loop = START;

    if (player == player1) { player_number = player2; }
    else { player_number = player1; }

    while (loop != END) {

        check = _getch();

        switch (check) {

        case UP:      // 위쪽 이동

            y = y - 1;
            if (y < 0) y = y + 1;
            ShowCursorPos(x, y);
            break;

        case DOWN:      // 아래쪽 이동

            y = y + 1;
            if (y >= MAPSIZE) y = y - 1;
            ShowCursorPos(x, y);
            break;

        case RIGHT:      // 오른쪽 이동

            x = x + 1;
            if (x >= MAPSIZE) x = x - 1;
            ShowCursorPos(x, y);
            break;

        case LEFT:      // 왼쪽 이동

            x = x - 1;
            if (x < 0) x = x + 1;
            ShowCursorPos(x, y);
            break;

        case RETURN:   // 수 무르기

            if (player_number == player1) {

                if (ret_count1 >= 2)
                    break;
                ret_count1++;
            }
            else if (player_number == player2) {
                if (ret_count2 >= 2)
                    break;
                ret_count2++;
            }

            ShowCursorPos(displayX, displayY);

            map[y][x] = CLEAR;
            play_count--;
            ShowCheckerBoard();

            if (player_number == player1) {
                printf("\n\n\n\n\n\n○(흑돌) 플레이어가 둘 차례입니다.");
            }
            else {
                printf("\n\n\n\n\n\n●(백돌) 플레이어가 둘 차례입니다.");
            }

            ShowCursorPos(x, y);
            InputSelect(player_number);

            if (player_number == player1)
                player_number = player2;
            else
                player_number = player1;

            ShowCheckerBoard();


            if (player == player1) {

                ShowCursorPos(displayX, displayY);
                printf("\n\n\n\n\n\n○(흑돌) 플레이어가 둘 차례입니다.");

            }
            else {

                ShowCursorPos(displayX, displayY);
                printf("\n\n\n\n\n\n●(백돌) 플레이어가 둘 차례입니다.");

            }

            GameStatus();
            ShowCursorPos(x, y);
            InputSelect(player_number);
            loop = END;

        case ENTER:      // 바둑돌을 착수할 때

            if (play_count == 0 && !(y == 0 || y == 7 || x == 0 || x == 7) && player == player1) {  // 가장자리에만 놓을 수 있게 처리
                STARTFLAG = 1;
                loop = END;
                break;
            }

            if (map[y][x] != NONE) break;

            map[y][x] = player;

            CheckMyStonePos(x, y, player);
            ClearStones();

            if (CheckPlayerHousePos(x, y, player_number) == YES) // 해당 위치에 집이 생겼을 경우, 둘 수 없으면 YES 출력 --> 두었던 것을 무효화
                map[y][x] = CLEAR;
            else
                loop = END;

            play_count++;
        }
    }
}

// 바둑돌을 착수했을 경우 그 바둑돌이 상대방 돌을 모두 포위하고 집을 만들 경우를 계속해서 검사하기 위한 재귀적 알고리즘
void CheckMyStonePos(int x, int y, int player_number) {

    ClearStones();      // 두었던 바둑을 초기화
    if (CheckPlayerHousePos(x + 1, y, player_number) == YES) RemoveStone(x + 1, y, player_number);

    ClearStones();
    if (CheckPlayerHousePos(x - 1, y, player_number) == YES) RemoveStone(x - 1, y, player_number);

    ClearStones();
    if (CheckPlayerHousePos(x, y + 1, player_number) == YES) RemoveStone(x, y + 1, player_number);

    ClearStones();
    if (CheckPlayerHousePos(x, y - 1, player_number) == YES) RemoveStone(x, y - 1, player_number);
}


int GameStatus() {      // 현재 게임의 진행 상태를 알려주는 함수

    int i, j;
    int player1_do = 0;
    int player2_do = 0;
    totalCount = 0;   // 돌의 총 수 저장

    for (i = 0; i < MAPSIZE; i++)
        for (j = 0; j < MAPSIZE; j++)
            if (map[i][j] != CLEAR)
                totalCount++;      // 둘 때마다 돌의 수를 1씩 증가

    ShowCursorPos(displayX, displayY + 3);

    printf("\n\n\n\n\n바둑돌을 둔 총 개수: %d\n1번 플레이어 : %d\n2번 플레이어 : %d\n\n돌을 무른 횟수(최대 2회까지 가능합니다)\n1번 플레이어 : %d\n2번 플레이어 : %d\n", totalCount, countReset1, countReset2, ret_count1, ret_count2);  // 돌의 총 개수, 플레이어1의 돌 수, 플레이어2의 돌 수 출력

    // 2번을 다 물렀을 때
    if (ret_count1 >= 2)
        printf("○(흑돌)은 더 이상 무를 수 없습니다.\n");
    if (ret_count2 >= 2)
        printf("●(백돌)은 더 이상 무를 수 없습니다.\n");

    if ((totalCount + countReset1 + countReset2) == MAPSIZE * MAPSIZE) {
        player1_do = Find(player1);
        player2_do = Find(player2);

        ShowCursorPos(displayX, displayY);

        if (countReset1 < countReset2)
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n축하합니다! \n●(백돌)이 승리하였습니다!!\n");
        else if (countReset1 > countReset2)
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n축하합니다! \n○(흑돌)이 승리하였습니다!!\n");
        else
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n무승부네요!\n");

        printf("1번 플레이어의 집 개수 : %d | 2번 플레이어의 집 개수 %d \n", player1_do + 2, player2_do);
        return 1;
    }
    return 0;

}

void ShowCheckerBoard() {                  // 화면 출력

    int x, y, n;
    char calculate[3];
    system("cls");


    for (y = 0; y < MAPSIZE; y++) {
        for (x = 0; x < MAPSIZE; x++) {
            if (map[y][x] == player1) {     // 플레이어1 - 흑돌
                printf("○");
            }
            else if (map[y][x] == player2) {// 플레이어2 - 백돌
                printf("●");
            }
            else if (map[y][x] == NONE) { // 바둑판 출력
                n = (x + 5) / 6 + (y + 5) / 6 * 3;
                calculate[0] = mapGrid[n * 2];
                calculate[1] = mapGrid[n * 2 + 1];
                calculate[2] = 0;               // 바둑판을 8x8 로 만들기 위한 계산 과정

                printf("%s ", calculate);         // 바둑판 출력
            }
        }
        printf("\n");
    }

    ShowCursorPos(displayX, displayLineY);
    printf("바둑을 시작합니다!\n");
    printf("키보드의 방향키로 위치 이동이 가능합니다.\n\n");
    printf("*********** 사용 방법 ***********\n바둑돌 착수 : spacebar 키를 입력해주세요!\n수 무르기 : backspace 키를 입력해주세요!\n");
}

int StoneFind(int x, int y, int player_number)
{
    int player_domain_count = 0;

    if (FindHouse(x + 1, y, player_number) == YES) {
        ClearStones();

        if (FindHouse(x - 1, y, player_number) == YES) {
            ClearStones();

            if (FindHouse(x, y + 1, player_number) == YES) {
                ClearStones();

                if (FindHouse(x, y - 1, player_number) == YES) {
                    player_domain_count++;
                }
            }
        }
    }
    return player_domain_count;
}

int Find(int player_number) {

    int i, j;
    int temp = 0;
    int count = 0;

    for (i = 0; i < MAPSIZE; i++) {
        for (j = 0; j < MAPSIZE; j++) {
            if (map[i][j] == 0) {
                temp = StoneFind(i, j, player_number);
                count += temp;
            }
        }
    }

    return count;
}

void ShowCursorPos(int x, int y) {
    COORD Pos = { x * 2 , y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void main() {

    int player = player1, GamePlayFlag = PLAY;
    char replayGo;
    ClearMap();
    ClearStones();
    ShowCheckerBoard();

   
    while (GamePlayFlag != END) {

        ShowCursorPos(displayX, displayY);

        if (STARTFLAG == 1) {
            printf("\n\n\n\n첫 수는 가장자리에만 놓을 수 있습니다.");
            STARTFLAG = 0;
            player = player1;
        }

        ShowCursorPos(displayX, displayY);

        if (player == player1) {
            printf("\n\n\n\n\n\n○(흑돌) 플레이어가 둘 차례입니다.");
        }
        else {
            printf("\n\n\n\n\n\n●(백돌) 플레이어가 둘 차례입니다.");
        }


        ShowCursorPos(x, y);
        InputSelect(player);
        ShowCheckerBoard();

        if (player == player1) {
            player = player2;
        }
        else {
            player = player1;
        }

        if (GameStatus() == OK) {
            GamePlayFlag = END;
        }
    }
}
