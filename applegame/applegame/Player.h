#pragma once
class Player {
private:
	int x, y;		//���Wx y
	int w, h;		//��w ����h
	int angle;	//�@�̂̌���
	int RestD;		//���D�̏����p
	int speed;		//�ړ����x

public:
	int flg;		//�g�p�t���O
	Player();
	void Init();
	void PlayerControl();	//���@����
	void AppleColision(int i);	//�����S�̓����蔻��
	int	 GetD();			//���D�̏���
};

extern Player player;