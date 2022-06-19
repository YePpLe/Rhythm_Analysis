// 2303 ���¿� 2022 ������ �м� ���α׷�  
// ����: ������� ���� ���� �м� ���α׷�  
// ���α׷� ����: 

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // pow, sqrt ���� �Լ� 
#include <windows.h> // ������ �ܼ�â ���� 
#include <process.h> // ���� ������  
#include <time.h> // �ð� ���� �� ��� ���� �������  
#include <conio.h> // _getch() ���  
#include <mmsystem.h> // �Ҹ� ��� 

void init_menu(); // �޴� ���� ȭ�� 
void arrowmove(int a, int b); // ����Ű ������ �Լ� ����Ա� 
void analysis(); // �� ���α׷��� �� ����� ���� �м� �� ��� ���� 
void load(); // ���� ����� �ҷ��� �� �� 
void gotoxy(int x, int y);

CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE}; // .Ŀ�� ���� 

int main()
{
	SetConsoleTitle("���� ���� �м� ���α׷�"); // ���α׷� ���� ���� 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); // Ŀ�� ����� 
    
	init_menu();
}

void init_menu()
{
	system("mode con:cols=51 lines=20"); // �ܼ�â ũ�� ���� 
	
	unsigned char input, current_pos = 8;
	
	printf("===================================================\n");
	printf("                                                   \n");
	printf("              ���� ���� �м� ���α׷�              \n");
	printf("                                       2303 ���¿� \n");
    printf("                                                   \n");
    printf("===================================================\n\n\n");
	printf("                   > ���� �м�                     \n"); // 8
	printf("                     ��� ��ȸ                     \n"); // 9
	printf("                       ����                        \n"); // 10
	
	// �޴� ���� ���� 
	while(1)
	{
		// �Է��� �޾��� �� 
		if(_kbhit()) {
			input = _getch();
			// ����Ű ������ ���� 
			if(input == 224 || input == 0) {
				input = _getch();
				switch(input) {
					case 72: // �� ����Ű 
						if(current_pos > 8) {
							arrowmove(current_pos, current_pos-1);
							current_pos-=1;
						}
						break;
					case 80: // �Ʒ� ����Ű 
						if(current_pos < 10) {
							arrowmove(current_pos, current_pos+1);
							current_pos+=1;
						}
						break;
				}
			}
			// ����Ű ���� 
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
	int cnt = 0, t; // cnt: �Է� Ƚ�� üũ,  t: ���� �� ���� 
	printf("���Ͻô� BPM�� �Է����ּ���(�ִ� 600BPM) : ");
	scanf("%d", &t); 
	t = 60000/t;
	unsigned int start_time, next_time, input_time[10] = {0, }, devitation[10] = {0, }; // start_time: ���� ���� �ð�, next_time: ���� ��� �ð�, input_time: �Է� �޾��� ���� �ð�  
	unsigned char input; // �� �Է��ߴ��� Ȯ�ο� 
	while(clock()%1000); // ù ��� �ð��� 1�� ������ ����� �ֱ� ���� ���� 
	next_time = clock();
	while(cnt < 10)
	{
		// ������ �������� ��Ʈ�γ� ����� ���� ���� 
		if(clock() >= next_time) {
			PlaySound("Metronome.wav", NULL, SND_ASYNC);
			next_time += t;
		} 
		// ������� �Է��� �޴� ���� 
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
	while(clock() <= next_time); // ���� �Ҹ� ��º��� ���� ������ �ȵ� �Ҹ� ��� 
	PlaySound("Metronome.wav", NULL, SND_ASYNC); // ���� ���� 
	
	printf("Ÿ�̹� : ��Ʈ�γ� ��� �Ҹ����� (+)�� ����, (-)�� ����\n");
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
	double avg = sumx/10.0, sd = sqrt((double)sumx2/10.0 - pow(avg, 2)); // avg: ���, sd: ǥ������ 
	printf("��� : %.1lfms, ǥ�� ���� : %.2lf", avg, sd);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
