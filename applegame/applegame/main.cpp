#include"DxLib.h"
#include"common.h"
#include"Player.h"
#include"Apple.h"
#include <math.h>

//�ϐ��̐錾
int g_OldKey;			//�O��̓��̓L�[
int g_NowKey;			//����̓��̓L�[
int g_KeyFlg;			//���̓L�[���

int g_GameState = 0;	//�Q�[�����[�h
int g_WaitTime;		//�҂�����

int g_Score;			//�X�R�A
int g_EnemyCount1;		//�G�J�E���g
int g_EnemyCount2;
int g_EnemyCount3;
int g_EnemyCount4;

int SpawnX[7] = { 0,0,0,0,0,0,0 };			//�����S�����ʒu(0:������ 1:�����ς�)

int g_TitleImage;		//�摜�p�ϐ�
int g_Menu, g_Cone;		//���j���[�摜�ϐ�
int g_RankingImage;		//�����L���O�摜�p�ϐ�
int g_Item[2];			//�A�C�e���摜�ϐ�
int g_EndImage;			//�G���h��ʗp�ϐ�
int g_Teki[4];			//�G�摜�ϐ�
int g_StageImage;		//�X�e�[�W�摜�ϐ�
int g_Car, g_Barrier;	//�L�����摜

//���@�����l

//�����L���O�f�[�^(�\����)
struct RankingData {
	int no;
	char name[11];
	long score;
};

//�����L���O�f�[�^�ϐ��錾
struct RankingData	g_Ranking[RANKING_DATA];

//�֐��̃v���g�^�C�v�錾
void GameInit(void);	//�Q�[������������
void GameMain(void);	//�Q�[�����C������

void DrawGameTitle(void);	//�^�C�g���`�揈��
void DrawGameOver(void);	//�Q�[���I�[�o�[��ʕ`�揈��
void DrawEnd(void);			//�Q�[���G���h�`�揈��
void DrawHelp(void);		//�Q�[���w���v�`�揈��

int CheckApple(void);		//�����S�̐����\���`�F�b�N
int SpawnAppleX(void);		//�o�����郊���S��X���W������

void BackScrool();		//�w�i�摜�X�N���[������

//int HitBoxPlayer(PLAYER *p, ENEMY *e);	//�����蔻��

void DrawRanking(void);		//�����L���O�`�揈��
void InputRanking(void);	//�����L���O����
void SortRanking(void);		//�����L���O���ёւ�
int  SaveRanking(void);		//�����L���O�f�[�^�̕ۑ�
int  ReadRanking(void);		//�����L���O�f�[�^�ǂݍ���

int LoadImages();			//�摜�ǂݍ���

//�v���O�����̊J�n
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("DriveGame");		//�^�C�g����ݒ�

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	if (LoadImages() == -1)return -1;

	if (ReadRanking() == -1)return -1;

	while (ProcessMessage() == 0 && g_GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {
	
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();	//��ʂ̏�����

		switch (g_GameState) {
		case 0:
			DrawGameTitle();	//�Q�[���^�C�g���`��
			break;

		case 1:
			GameInit();			//�Q�[����������
			break;

		case 2:
			DrawRanking();		//�����L���O�`��
			break;

		case 3:
			DrawHelp();			//�Q�[���w���v�`��
			break;

		case 4:
			DrawEnd();			//�Q�[���G���h�`��
			break;

		case 5:
			GameMain();			//�Q�[�����C������
			break;

		case 6:
			DrawGameOver();		//�Q�[���I�[�o�[�`��
			break;

		case 7:
			InputRanking();		//�����L���O����
			break;
		}
		ScreenFlip();			//����ʂ̓��e��\��ʂɔ��f
	}
	DxLib_End();				//DX���C�u�����d�l�̏I������

	return 0;					//�\�t�g�̏I��

}

//�Q�[���^�C�g���\��
void DrawGameTitle(void) {
	static int MenuNo = 0;

	if (MenuNo > 3 || MenuNo < 0) MenuNo = 0;

	
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 3)MenuNo = 0;
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 3;
	}

	//Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_A)g_GameState = MenuNo + 1;

	//�^�C�g���摜�\��
	DrawGraph(0, 0, g_TitleImage, FALSE);

	//���j���[
	DrawGraph(120, 200, g_Menu, TRUE);

	//���j���[�J�[�\��
	DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, 0, g_Cone, TRUE);
}

//�Q�[����������
void GameInit(void) {

	//���ԏ�����
	g_WaitTime = 0;

	//�X�R�A������
	g_Score = 0;

	//�G�̏����ݒ�
	g_EnemyCount1 = 0;
	g_EnemyCount2 = 0;
	g_EnemyCount3 = 0;
	g_EnemyCount4 = 0;

	//�v���C���[�����ݒ�
	player.Init();

	//�G�̏����ݒ�
	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].flg = FALSE;
	}

	//�Q�[�����C��������
	g_GameState = 5;
}

//�Q�[�������L���O��ʕ\��
void DrawRanking(void) {

	//�X�y�[�X�L�[�Ń��j���[�ɖ߂�
	if (g_KeyFlg & PAD_INPUT_M)g_GameState = 0;

	//�����L���O��ʕ\��
	DrawGraph(0, 0, g_RankingImage, FALSE);

	//�����L���O�ꗗ��\��
	SetFontSize(18);
	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	DrawString(100, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);

}

//�Q�[���w���v�`��
void DrawHelp(void) {

	//�X�y�[�X�L�[�Ń��j���[�ɖ߂�
	if (g_KeyFlg & PAD_INPUT_M)g_GameState = 0;

	//�^�C�g���摜�\��
	DrawGraph(0, 0, g_TitleImage, FALSE);
	SetFontSize(16);
	DrawString(20, 120, "- HELP -", 0xffffff, 0);

	DrawString(20, 160, "��Q��������Ȃ���", 0xffffff, 0);
	DrawString(20, 180, "���葱���悤", 0xffffff, 0);
	DrawString(20, 200, "�R�����s���邩", 0xffffff, 0);
	DrawString(20, 220, "��Q���ɐ��񓖂���ƃQ�[���I�[�o�[", 0xffffff, 0);
	DrawString(20, 250, "�A�C�e���ꗗ", 0xffffff, 0);
	DrawGraph(20, 260, g_Item[0], TRUE);
	DrawString(20, 315, "�R�����񕜂ł���", 0xffffff, 0);
	DrawGraph(20, 335, g_Item[1], TRUE);
	DrawString(20, 385, "�ϋv���񕜂ł���", 0xffffff, 0);
	DrawString(20, 405, "�����Ȃ�R���������񕜂ł���", 0xffffff, 0);
	DrawString(150, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);

}

//�Q�[���G���h�`��
void DrawEnd(void) {

	//�G���h��ʕ\��
	DrawGraph(0, 0, g_EndImage, FALSE);

	SetFontSize(24);
	DrawString(360, 480 - 24, "Thankyou for Playing", 0xffffff, 0);

	//�^�C�����Z����&�I��(3�b)
	if (++g_WaitTime > 180)g_GameState = 99;

}

//�Q�[�����C��
void GameMain(void) {

	static int restapple = CheckApple();	//�����S�����\��

	BackScrool();

	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].EnemyControl();	//�����S����
	}
	if (g_WaitTime++ % 25 == 0&& g_WaitTime % 25 != 0) {	//25f���ƂɃ����S�o��
		for (int i = 0; i < restapple / 2; i++) {		//�����S�̐������`�F�b�N(�����\��/2)
			for (int j = i; j < APPLE_MAX; j++) {			
				if (apple[j].flg == FALSE) {			//apple[j]�̂�񂲂��o������(�t���O��TRUE��)
					apple[j].Spawn(SpawnAppleX());		//��o��(�t���O��FALSE)�Ȃ�X���W�����肵�����S���o��������
					j += APPLE_MAX;
				}
			}
		}
		for (int k = 0; k < 7; k++) {
			SpawnX[k] = 0;				//�����S�����ʒu�����Z�b�g
		}
		restapple = CheckApple();
	}

	player.PlayerControl();	//�v���C���[����

}

//�����S�̐����\���`�F�b�N
int CheckApple(void) {
	int rest = 0;

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].flg == FALSE) rest++;
	}
	return rest;
}

//�o�����郊���S��X���W������
int SpawnAppleX(void) {
	int x = 0;

	do  {
		x = GetRand(6);			//X���W�������_���I��
	} while (SpawnX[x] != 0);	//���̈ʒu�ɂ��łɐ�������Ă���H

	SpawnX[x] = 1;
	return x;
}

//�w�i�摜�X�N���[������
void BackScrool() {

	//�X�e�[�W�摜�\��

	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, 500, 480);

	DrawGraph(0, 0, g_StageImage, FALSE);

	//�G���A��߂�
	SetDrawArea(0, 0, 640, 480);

	//�X�R�A���\���̈�
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
}

//�G�Ƃ̓����蔻��
//int HitBoxPlayer(player *p, apple *e) {
//	//x,y�͒��S���W
//	int sx1 = p->x - (p->w / 2) + 5;
//	int sy1 = p->y - (p->h / 2) + 5;
//	int sx2 = sx1 + p->w - 10;
//	int sy2 = sy1 + p->h - 10;
//
//	int dx1 = e->x - (e->w / 2) + 5;
//	int dy1 = e->y - (e->h / 2) + 5;
//	int dx2 = dx1 + e->w - 10;
//	int dy2 = dy1 + e->h - 10;
//
//	//��`���d�Ȃ�Γ�����
//	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
//		return TRUE;
//	}
//	return FALSE;
//}
// 
//�Q�[���I�[�o�[��ʕ`��

void DrawGameOver(void) {

	BackScrool();

	//�X�y�[�X�L�[�Ń��j���[�ɖ߂�
	if (g_KeyFlg & PAD_INPUT_M) {
		if (g_Ranking[RANKING_DATA-1].score >= g_Score) {
			g_GameState = 0;
		}
		else {
			g_GameState = 7;
		}
	}

	DrawString(150, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);
}

 //�����L���O���͏���
void InputRanking(void)
{
	//�����L���O�摜�\��
	DrawGraph(0, 0, g_RankingImage, FALSE);

	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(20);

	// ���O���͎w��������̕`��
	DrawString(150, 240, "�����L���O�ɓo�^���܂�", 0xFFFFFF);
	DrawString(150, 270, "���O���p���œ��͂��Ă�������", 0xFFFFFF);

	// ���O�̓���
	DrawString(150, 310, "> ", 0xFFFFFF);
	DrawBox(160, 305, 300, 335, 0x000055, TRUE);
	if (KeyInputSingleCharString(170, 310, 10, g_Ranking[4].name, FALSE) == 1) {
		g_Ranking[4].score = g_Score;	// �����L���O�f�[�^��5�ԖڂɃX�R�A��o�^
		SortRanking();		// �����L���O���בւ�
		SaveRanking();		// �����L���O�f�[�^�̕ۑ�
		g_GameState = 2;		// �Q�[�����[�h�̕ύX
	}

}

 //�����L���O���בւ�
void SortRanking(void)
{
	int i, j;
	RankingData work;

	// �I��@�\�[�g
	for (i = 0; i < 9; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// ���ʕt��
	for (i = 0; i < 10; i++) {
		g_Ranking[i].no = 1;
	}
	// ���_�������ꍇ�́A�������ʂƂ���
	// �����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
	for (i = 0; i < 9; i++) {
		for (j = i + 1; j < 10; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}

 //�����L���O�f�[�^�̕ۑ�
int  SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// �t�@�C���I�[�v��
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* �G���[���� */
		printf("Ranking Data Error\n");
		return -1;
	}

	// �����L���O�f�[�^���z��f�[�^����������
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;

}

 //�����L���O�f�[�^�ǂݍ���
int ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//�t�@�C���I�[�v��
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//�G���[����
		printf("Ranking Data Error\n");
		return -1;
	}

	//�����L���O�f�[�^�z����f�[�^��ǂݍ���
	for (int i = 0; i < RANKING_DATA; i++) {
		fscanf(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;
}

int LoadImages() {

	//�^�C�g��
	if ((g_TitleImage = LoadGraph("images/Title.bmp")) == -1)return -1;

	//���j���[
	if ((g_Menu = LoadGraph("images/menu.bmp")) == -1)return -1;
	if ((g_Cone = LoadGraph("images/leaf.png")) == -1)return -1;

	//�����L���O
	if ((g_RankingImage = LoadGraph("images/Ranking.bmp")) == -1)return -1;

	//�X�e�[�W�w�i
	if ((g_StageImage = LoadGraph("images/back.png")) == -1)return -1;

	//�v���C���[
	if ((g_Car = LoadGraph("images/car1pol.bmp")) == -1)return -1;

	//�G
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 40, 40, g_Teki) == -1)return -1;

	//�G���f�B���O
	if ((g_EndImage = LoadGraph("images/End.bmp")) == -1)return -1;

	return 0;
}
