#include"Apple.h"
#include"common.h"
#include"Dxlib.h"
#include"Math.h"
#include"Player.h"
extern int g_Teki[3];
Apple apple[APPLE_MAX];

Apple::Apple() {
	flg = FALSE;	//�g�p�t���O
	type = 0;	//�^�C�v
	img = 0;	//�摜
	x = 0;	//���Wxy ��w ����h
	y = -50;
	w = 40;
	h = 40;
	speed = 0;	//���x
	point = 0;	//�X�R�A���Z
}

void Apple::Spawn(int x) {
	flg = TRUE;	//�g�p�t���O
	type = GetRand(3);	//�^�C�v
	img = g_Teki[type];	//�摜
	this->x = x * 62 + 62;	//���Wxy ��w ����h
	y = -50;
	w = 40;
	h = 40;
	speed = type + 1;	//���x
	point = 0;	//�X�R�A���Z
}

void Apple::EnemyControl() {
		if (flg == TRUE) {
			//�G�̕\��
			DrawGraph(x, y, img, TRUE);

			//�^���Ɉړ�
			y += speed;

			//��ʂ��o��Ə���
			if (y > SCREEN_HEIGHT + h)flg = FALSE;
		}
	
}