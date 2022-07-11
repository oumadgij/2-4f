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

	//左右移動
	if (g_NowKey & PAD_INPUT_LEFT) speed -= kansei;
	if (g_NowKey & PAD_INPUT_RIGHT) speed += kansei;
	//スピードの制限
	if (speed > PLAYER_MAX_SPEED) speed = PLAYER_MAX_SPEED;
	if (speed < -PLAYER_MAX_SPEED) speed = -PLAYER_MAX_SPEED;
	//入力を止めた時の処理
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


	//画像幅と高さの更新
	if (speed != 0) {
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

	if (RestD > 0) {	//りんごDの効果時間をカウントする
		RestD--;
	}
}

void Player::AppleColision(int i) {

		if (apple[i].flg == TRUE) {	//そのリンゴは出現中？

			int px1 = 0;		//プレイヤーX座標始点
			int py1 = 0;		//プレイヤーY座標始点
			int px2 = 0;	//プレイヤーX座標終点
			int py2 = 0;	//プレイヤーY座標終点

			if (speed == 0) {
				px1 = x + 3;		//プレイヤーX座標始点
				py1 = y + 1;		//プレイヤーY座標始点
				px2 = px1 + w - 3;	//プレイヤーX座標終点
				py2 = py1 + h;	//プレイヤーY座標終点
			}
			else {
				switch (angle)
				{
				case LEFT:
					px1 = x;		//プレイヤーX座標始点
					py1 = y;		//プレイヤーY座標始点
					px2 = px1 + w - 23;	//プレイヤーX座標終点
					py2 = py1 + h;	//プレイヤーY座標終点
					break;

				case RIGHT:
					px1 = x + 23;		//プレイヤーX座標始点
					py1 = y;		//プレイヤーY座標始点
					px2 = px1 + w - 23;	//プレイヤーX座標終点
					py2 = py1 + h;	//プレイヤーY座標終点
					break;
				}
				
			}

			int ax1 = apple[i].GetX() + 7;	//りんごX始点
			int ay1 = apple[i].GetY() + 3;	//りんごY始点
			int ax2 = ax1 + apple[i].GetWidth() - 14;			//りんごX終
			int ay2 = ay1 + apple[i].GetHeight() - 10;			//りんごY終点

			//矩形が重なれば当たり
			if (px1 < ax2 && px2 > ax1 && py1 < ay2 && py2 > ay1) {


				apple[i].AppleCount();	//アップルカウント

				if (apple[i].GetType() == 3){
					//RestD = 120;	//りんごDを取ったらペナルティの効果時間(120F)をセット
//idou
					PlaySoundMem(g_PoisonSE, DX_PLAYTYPE_BACK, TRUE);  //りんごD取得時のSE
				}
				else {
					PlaySoundMem(g_CatchSE, DX_PLAYTYPE_BACK, TRUE);  //りんごA～C取得時のSE
				}
				apple[i].flg = FALSE;

			}
		}
}

//りんごDの処理
int Player::GetD() {
	return RestD;	//残り効果時間を返す
}

void Player::setRestD(int time) {
	RestD = time;		//りんごDを取ったらペナルティの効果時間(120F)をセット
}