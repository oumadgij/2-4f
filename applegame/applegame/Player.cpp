#include"Player.h"
#include"Dxlib.h"
#include"common.h"
#include"Math.h"
extern int g_PlayerImage[2];
typedef enum Angle {
	LEFT,
	RIGHT
};

Player player;

Player::Player() {
	flg = TRUE;
	w = 40;
	h = 100;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - h;
	angle = 0.0;
	count = 0;
	speed = 5;
}

void Player::Init() {
	flg = TRUE;
	w = 40;
	h = 100;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - h;
	angle = LEFT;
	count = 0;
	speed = 5;
}

void Player::PlayerControl() {

	//���E�ړ�
	if (g_NowKey & PAD_INPUT_LEFT)x -= speed;
	if (g_NowKey & PAD_INPUT_RIGHT)x += speed;

	//�摜���ƍ����̍X�V
	if (g_NowKey & PAD_INPUT_LEFT || g_NowKey & PAD_INPUT_RIGHT) {
		w = 60;
		h = 80;
	}
	else {
		w = 40;
		h = 100;
	}
	y = SCREEN_HEIGHT - h;

	//��ʂ���o�Ȃ��悤�ɂ���
	if (x < 0)x = 0;

	if (x > SCREEN_WIDTH - (140 + w))x = SCREEN_WIDTH - (140 + w);

	//�v���C���[�̕\��
	if (g_NowKey & PAD_INPUT_LEFT) {
		angle = LEFT;
		DrawGraph(x, y, g_PlayerImage[1], TRUE);
	}
	else if (g_NowKey & PAD_INPUT_RIGHT) {
		angle = RIGHT;
		DrawTurnGraph(x, y, g_PlayerImage[1], TRUE);
	}
	else {
		switch (angle)
		{
		case LEFT:
			DrawGraph(x, y, g_PlayerImage[0], TRUE);
			break;

		case RIGHT:
			DrawTurnGraph(x, y, g_PlayerImage[0], TRUE);
			break;
		}
	}
}