#pragma once
#include"common.h"

class Apple {
private:
	int type;	//�^�C�v
	int img;	//�摜
	int x, y, w, h;	//���Wxy ��w ����h
	int speed;	//���x
	int point;	//�X�R�A���Z

public:
	int flg;	//�g�p�t���O
	Apple();
	void Spawn(int x);			//�����S�o�� x=�o������X���W�̑��ΓI�Ȉʒu
	void EnemyControl();	//�G�@����
};

extern Apple apple[APPLE_MAX];