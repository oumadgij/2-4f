#pragma once
#define	_USE_MATH_DEFINES
#define	RANKING_DATA	5

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int APPLE_MAX = 10;

const int TIMELIMIT = 30000;

extern int g_StartTime;		//�X�^�[�g����
extern int g_WaitTime;		//�҂�����
extern int g_GameState;		// �Q�[���X�e�[�^�X

extern int g_OldKey;			//�O��̓��̓L�[
extern int g_NowKey;			//����̓��̓L�[
extern int g_KeyFlg;			//���̓L�[���

extern int g_Score;			//�X�R�A
extern int g_AppleCount[3];		//�G�J�E���g

extern int FontHandle;