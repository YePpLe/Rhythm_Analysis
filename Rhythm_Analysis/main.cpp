// 2303 ���¿� 2022 ������ �м� ���α׷�  
// ����: ������� ���� ���� �м� ���α׷�  
// ����: https://coding-factory.tistory.com/category/Language/C%20%20%2C%20%20%20C%2B%2B%20%20%2C%20%20C%23
// ���α׷� ����: 
// �ִ� 200BPM ���� ���� �����ݵ� 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // bool �ڷ���  
#include <windows.h> // ������ �ܼ�â ���� 
//#pragma comment(lib, "winmm.lib")
#include <process.h> // ���� ������  
#include <time.h> // �ð� ���� �� ��� ���� �������  
#include <conio.h> // _getch() ���  
#include <mmsystem.h> // �Ҹ� ��� 

MCI_OPEN_PARMS openMetronomeSound;
MCI_PLAY_PARMS playMetronomeSound;
int dwID;


void menu(); // �޴� ���� ȭ�� 
void analysis(); // �� ���α׷��� �� ����� ���� �м� �� ��� ���� 
void load(); // ���� ����� �ҷ��� �� �� 
void playingMetronomeSound(void* n); // ������� ���� ��� 

bool playing = false;

int main()
{
	analysis();
	system("pause");
}

void analysis()
{
	int i = 0, t = 300;
	long long start_time = clock(), input_time[10] = {0, };
	unsigned char input = 0;
	playing = true;
	_beginthread(playingMetronomeSound, 0, (void*)t);
	while(i < 10)
	{
		if(_kbhit()) {
			input = _getch();
			input_time[i] = clock();
			i++;
		}
	}
	playing = false;
	printf("���� �ð�: %d\n", start_time);
	for(i = 0; i < 10; i++)
		printf("%d ", input_time[i]);
}

void playingMetronomeSound(void* n)
{
	Sleep(1000);
	while(playing) {
		printf("%d\n", clock());
		PlaySound("Metronome.wav", NULL, SND_ASYNC);
		Sleep((int)n-10);
	}
}
