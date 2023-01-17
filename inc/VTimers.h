/////////////////////////////////////////////////////
//������������ ���� ���������� ����������� ��������//
//�����: ������� �.�.                              //
// - ���������� #include "VTimers.h                //
// - �������������� ������� ������ �������         //
// - �������������� ������ MAX_TIMERS              //
// - � handler ��������� �������:                  //
//           InterruptVTimerHandler();             //
//           MillisHandler();                      //
//           MicrosHandler();                      //
/////////////////////////////////////////////////////

#ifndef VTIMER_h
#define VTIMER_h

//����������� ������ �������
//������� ���������� �� ������������ �������-�������� 1953,125 ��
#define msec 1      // 1�� = 1 ����    
#define sec 1000    // 1� =  1000 ������ 
#define min 60000   // MAX 64 sec
#define hour 60*min 
#define day 24*hour

#define MAX_TIMERS 10 //���-�� ����������� ��������
//Timers ID:
#define VTIMER1   0  
#define VTIMER2   1  
#define VTIMER3   2  
#define VTIMER4   3  
#define VTIMER5   4  
#define VTIMER6   5  
#define VTIMER7   6  
#define VTIMER8   7  
#define VTIMER9   8  
#define VTIMER10  9  
#define VTIMER11  10
#define VTIMER12  11
#define VTIMER13  12
#define VTIMER14  13
#define VTIMER15  14
#define VTIMER16  15

//������� ������ � ���������:
void InitVTimers(void);                       //������������� ��������
void StartVTimer(unsigned char Timer);        //������ ������������ �������
void StopVTimer(unsigned char Timer);         //����������+��������
void ResetVTimer(unsigned char Timer);        //����� �������� �������
void PauseVTimer(unsigned char Timer);        //�����
void ReleaseVTimer(unsigned char Timer);      //����������
unsigned int GetVTimer(unsigned char Timer);  //������� ������� �������� �������
unsigned long millis(void);   
unsigned long micros(void);   

// ���������� ���������� �������:
//    - �������� � ���������� ���������� ��������� �������/��������
//    - ������ ���������� ��������� ������� = 1 �������� ������������ �������
void InterruptVTimerHandler(void);   
void MillisHandler(void);          //���������� ��������� ������ �� 1 ��� = 1 ��
void MicrosHandler(void);          //���������� ��������� ������ �� 1 ��� = 1 ���

#endif

