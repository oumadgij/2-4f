#pragma once
class Player {
private:
	int x, y;		//���Wx y
	int w, h;		//��w ����h
	double angle;	//�@�̂̌���
	int count;		//�^�C�~���O�p
	int speed;		//�ړ����x
	int hp;			//�̗�
	int fuel;		//�R��
	int bari;		//�o���A��
	int baricnt;	//�o���A�p������
	int bariup;		//�o���A��(���s�����A�b�v)

public:
	int flg;		//�g�p�t���O
	Player();
	void Init();
	void PlayerControl();	//���@����
};

extern Player player;