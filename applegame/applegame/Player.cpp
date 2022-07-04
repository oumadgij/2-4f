#include"Player.h"
#include"Dxlib.h"
#include"common.h"
#include"Math.h"
#include"Apple.h"
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

	//左右移動
	if (g_NowKey & PAD_INPUT_LEFT)x -= speed;
	if (g_NowKey & PAD_INPUT_RIGHT)x += speed;

	//画像幅と高さの更新
	if (g_NowKey & PAD_INPUT_LEFT || g_NowKey & PAD_INPUT_RIGHT) {
		w = 60;
		h = 80;
	}
	else {
		w = 40;
		h = 100;
	}
	y = SCREEN_HEIGHT - h;

	//画面から出ないようにする
	if (x < 0)x = 0;

	if (x > SCREEN_WIDTH - (140 + w))x = SCREEN_WIDTH - (140 + w);

	//プレイヤーの表示
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

int Player::AppleColision(int i) {
		if (apple[i].flg == TRUE) {	//そのリンゴは出現中？
			int px1 = x;		//プレイヤーX座標始点
			int py1 = y;		//プレイヤーY座標始点
			int px2 = px1 + w;	//プレイヤーX座標終点
			int py2 = py1 + h;	//プレイヤーY座標終点

			int ax1 = apple[i].GetX();	//りんごX始点
			int ay1 = apple[i].GetY();	//りんごY始点
			int ax2 = ax1 + apple[i].GetWidth();			//りんごX終
			int ay2 = ay1 + apple[i].GetHeight();			//りんごY終点

			//矩形が重なれば当たり
			if (px1 < ax2 && px2 > ax1 && py1 < ay2 && py2 > ay1) {
				apple[i].flg = FALSE;
				return apple[i].GetType();
			}
		}
		return 4;
}