#pragma once
#define	_USE_MATH_DEFINES
#define	RANKING_DATA	5
#define hold	65535/4

#include "DxLib.h"

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int APPLE_MAX = 10;
const int TIMELIMIT = 30;

extern int g_TimeLimit;		//�X�^�[�g����
extern int g_WaitTime;		//�҂�����
extern int g_GameState;		// �Q�[���X�e�[�^�X

extern XINPUT_STATE g_OldKey;			//�O��̓��̓L�[
extern XINPUT_STATE g_NowKey;			//����̓��̓L�[

extern int g_Score;			//�X�R�A
extern int g_AppleCount[3];		//�G�J�E���g

extern int FontHandle1;
extern int FontHandle2;
extern int FontHandle3;

extern int g_CatchSE;			//A,B,C�̃����S�擾����SE
extern int g_PoisonSE;			//D�̃����S�擾����SE