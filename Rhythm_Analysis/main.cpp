// 2303 남태웅 2022 데이터 분석 프로그램  
// 주제: 사용자의 박자 감각 분석 프로그램  
// 프로그램 설명: 

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // pow, sqrt 등의 함수 
#include <windows.h> // 윈도우 콘솔창 구성 
#include <process.h> // 다중 스레드  
#include <time.h> // 시간 측정 및 제어를 위한 헤더파일  
#include <conio.h> // _getch() 사용  
#include <mmsystem.h> // 소리 출력 

void init_menu(); // 메뉴 구성 화면 
void arrowmove(int a, int b); // 방향키 움직임 함수 우려먹기 
void analysis(); // 이 프로그램의 주 기능인 박자 분석 및 기록 저장 
void load(); // 과거 기록을 불러온 후 비교 
void gotoxy(int x, int y);

CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE}; // .커서 설정 

int main()
{
	SetConsoleTitle("박자 감각 분석 프로그램"); // 프로그램 제목 지정 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // 커서 숨기기 
    
	init_menu();
}

void init_menu()
{
	system("mode con:cols=51 lines=20"); // 콘솔창 크기 설정 
	
	unsigned char input, current_pos = 8;
	
	printf("===================================================\n");
	printf("                                                   \n");
	printf("              박자 감각 분석 프로그램              \n");
	printf("                                       2303 남태웅 \n");
    printf("                                                   \n");
    printf("===================================================\n\n\n");
	printf("                   > 박자 분석                     \n"); // 8
	printf("                     기록 조회                     \n"); // 9
	printf("                       종료                        \n"); // 10
	
	// 메뉴 선택 구현 
	while(1)
	{
		// 입력을 받았을 때 
		if(_kbhit()) {
			input = _getch();
			// 방향키 움직임 구현 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // 위 방향키 
						if(current_pos > 8) {
							arrowmove(current_pos, current_pos-1);
							current_pos-=1;
						}
						break;
					case 80: // 아래 방향키 
						if(current_pos < 10) {
							arrowmove(current_pos, current_pos+1);
							current_pos+=1;
						}
						break;
				}
			}
			// 엔터키 구현 
			else if(input == 13) {
				switch(current_pos) {
					case 8:
						analysis();
						break;
					case 10:
						return;
						break;
				}
			}
		}
	}
}

void arrowmove(int a, int b)
{
	if(a == 10)
		gotoxy(21, a);
	else
		gotoxy(19, a);
	printf(" ");
	if(b == 10)
		gotoxy(21, b);
	else
		gotoxy(19, b);
	printf(">");
}

void analysis()
{
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	system("mode con:cols=70 lines=20");
	system("cls");
	int cnt = 0, t; // cnt: 입력 횟수 체크,  t: 박자 간 간격 
	printf("원하시는 BPM을 입력해주세요(최대 600BPM) : ");
	scanf("%d", &t); 
	t = 60000/t;
	unsigned int start_time, next_time, input_time[10] = {0, }, devitation[10] = {0, }; // start_time: 최초 시작 시간, next_time: 다음 재생 시간, input_time: 입력 받았을 때의 시간  
	unsigned char input; // 뭐 입력했는지 확인용 
	while(clock()%1000); // 첫 출력 시간을 1초 단위로 만들어 주기 위한 과정 
	next_time = clock();
	while(cnt < 10)
	{
		// 일정한 간격으로 메트로놈 출력을 위한 구문 
		if(clock() >= next_time) {
			PlaySound("Metronome.wav", NULL, SND_ASYNC);
			next_time += t;
		} 
		// 사용자의 입력을 받는 구문 
		if(_kbhit()) {
			input_time[cnt] = clock();
			input = _getch();
			if(input == 27) {
				analysis();
				return;
			}
			if(!cnt) {
				start_time = input_time[cnt]-(next_time-t) < next_time-input_time[cnt] ? next_time-t : next_time;
			}
			cnt++;
		}
	}
	while(clock() <= next_time); // 원래 소리 출력보다 빨리 눌러서 안된 소리 출력 
	PlaySound("Metronome.wav", NULL, SND_ASYNC); // 위와 같음 
	
	printf("타이밍 : 메트로놈 출력 소리보다 (+)는 느림, (-)는 빠름\n");
	for(int i = 0; i < 10; i++)
	{
		devitation[i] = input_time[i]-start_time;
		printf("%dms ", devitation[i]); 
		start_time += t;
	}
	printf("\n");
	
	int sumx = 0, sumx2 = 0;
	for(int i = 0; i < 10; i++) {
		sumx += devitation[i];
		sumx2 += devitation[i]*devitation[i];
		printf("%d\n", sumx2);
	}
	double avg = sumx/10.0, sd = sqrt((double)sumx2/10.0 - pow(avg, 2)); // avg: 평균, sd: 표준편차 
	printf("평균 : %.1lfms, 표준 편차 : %.2lf", avg, sd);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
