#pragma once
class Player {
private:
	float x;    //���W x
	int y;		//���W y
	int w, h;		//��w ����h
	int angle;	//�@�̂̌���
	int RestD;		//���D�̏����p
	float speed;	//�ړ����x
	float kansei;   //����

public:
	int flg;		//�g�p�t���O
	Player();
	void Init();
	void PlayerControl();	//���@����
	void AppleColision(int i);	//�����S�̓����蔻��
	int	 GetD();			//���D�̏���
};

extern Player player;