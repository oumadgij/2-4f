#pragma once
class Player {
private:
	int x, y;		//���Wx y
	int w, h;		//��w ����h
	double angle;	//�@�̂̌���
	int count;		//�^�C�~���O�p
	int speed;		//�ړ����x

public:
	int flg;		//�g�p�t���O
	Player();
	void Init();
	void PlayerControl();	//���@����
};

extern Player player;