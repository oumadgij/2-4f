#pragma once
class Player {
private:
	int y;			//���Wy
	float x;		//���Wx
	int w, h;		//��w ����h
	int angle;	//�@�̂̌���
	int RestD;		//���D�̏����p
	float speed;    //�ړ����x
	float kansei;	//����	

public:
	int flg;		//�g�p�t���O
	Player();
	void Init();
	void PlayerControl();	//���@����
	void AppleColision(int i);	//�����S�̓����蔻��
	int	 GetD();			//���D�̏���
	void setRestD(int time);
};

extern Player player;