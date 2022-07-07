#include"Player.h"
#include"Dxlib.h"
#include"common.h"
#include"Math.h"
#include"Apple.h"

#define PLAYER_MAX_SPEED 4

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
	RestD = 0;
	speed = 0.0f;
	kansei = 0.1f;
}

void Player::Init() {
	flg = TRUE;
	w = 40;
	h = 100;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - h;
	angle = LEFT;
	RestD = 0;
	speed = 0.0f;
	kansei = 0.1f;
}

void Player::PlayerControl() {

	//���E�ړ�
	if (g_NowKey & PAD_INPUT_LEFT) speed -= kansei;
	if (g_NowKey & PAD_INPUT_RIGHT) speed += kansei;
	//�X�s�[�h�̐���
	if (speed > PLAYER_MAX_SPEED) speed = PLAYER_MAX_SPEED;
	if (speed < -PLAYER_MAX_SPEED) speed = -PLAYER_MAX_SPEED;
	//���͂��~�߂����̏���
	if ((g_NowKey & PAD_INPUT_LEFT) == 0 && (g_NowKey & PAD_INPUT_RIGHT) == 0)
	{
		if (speed < -0.09f)
		{
			speed += 0.09f;
		}
		else if (speed > 0.09f)
		{
			speed -= 0.09f;
		}
		else
		{
			speed = 0.0f;
		}
	}

	x += speed;


	//�摜���ƍ����̍X�V
	if (speed != 0) {
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
	if ((RestD / 20) % 2 == 0) {
		if (speed < 0) {
			angle = LEFT;
			DrawGraph((int)x, y, g_PlayerImage[1], TRUE);
		}
		else if (speed > 0) {
			angle = RIGHT;
			DrawTurnGraph((int)x, y, g_PlayerImage[1], TRUE);
		}
		else {
			switch (angle)
			{
			case LEFT:
				DrawGraph((int)x, y, g_PlayerImage[0], TRUE);
				break;

			case RIGHT:
				DrawTurnGraph((int)x, y, g_PlayerImage[0], TRUE);
				break;
			}
		}
	}

	if (RestD > 0) {	//���D�̌��ʎ��Ԃ��J�E���g����
		RestD--;
	}

	//�v�����Ԃ��߂�����Q�[���I�[�o�[
	int Time = TIMELIMIT - (GetNowCount() - g_StartTime);
	if (Time <= 0) {
		g_GameState = 6;
	}
	DrawFormatStringToHandle(495, 50, 0xffffff, FontHandle, "%3d", Time / 1000 + 1);
}

void Player::AppleColision(int i) {
		if (apple[i].flg == TRUE) {	//���̃����S�͏o�����H
			int px1 = x;		//�v���C���[X���W�n�_
			int py1 = y;		//�v���C���[Y���W�n�_
			int px2 = px1 + w;	//�v���C���[X���W�I�_
			int py2 = py1 + h;	//�v���C���[Y���W�I�_

			int ax1 = apple[i].GetX();	//���X�n�_
			int ay1 = apple[i].GetY();	//���Y�n�_
			int ax2 = ax1 + apple[i].GetWidth();			//���X�I
			int ay2 = ay1 + apple[i].GetHeight();			//���Y�I�_

			//��`���d�Ȃ�Γ�����
			if (px1 < ax2 && px2 > ax1 && py1 < ay2 && py2 > ay1) {
				if (apple[i].GetType() == 3) RestD = 120;	//���D���������y�i���e�B�̌��ʎ���(120F)���Z�b�g
				apple[i].flg = FALSE;
			}
		}
}

//���D�̏���
int Player::GetD() {
	return RestD;	//�c����ʎ��Ԃ�Ԃ�
}