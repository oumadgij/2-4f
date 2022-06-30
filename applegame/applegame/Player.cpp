#include"Player.h"
#include"Dxlib.h"
#include"common.h"
#include"Math.h"
extern int g_Car;

Player player;

Player::Player() {
	flg = TRUE;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 100;
	w = 60;
	h = 120;
	angle = 0.0;
	count = 0;
	speed = 5;
	hp = 1000;
	fuel = 20000;
	bari = 3;
	bariup = 10;
}

void Player::Init(){
	flg = TRUE;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 100;
	w = 60;
	h = 120;
	angle = 0.0;
	count = 0;
	speed = 5;
	hp = 1000;
	fuel = 20000;
	bari = 3;
	bariup = 10;
}

void Player::PlayerControl() {
	//�㉺���E�ړ�
		if (g_NowKey & PAD_INPUT_UP)y -= speed;
		if (g_NowKey & PAD_INPUT_DOWN)y += speed;
		if (g_NowKey & PAD_INPUT_LEFT)x -= speed;
		if (g_NowKey & PAD_INPUT_RIGHT)x += speed;

	//��ʂ���o�Ȃ��悤�ɂ���
	if (x < 32)x = 32;

	if (x > SCREEN_WIDTH - 180)x = SCREEN_WIDTH - 180;

	if (y < 60)y = 60;

	if (y > SCREEN_HEIGHT - 60)y = SCREEN_HEIGHT - 60;

	//�v���C���[�̕\��
	DrawRotaGraph(x, y, 1.0f, 0, g_Car, TRUE, FALSE);
}