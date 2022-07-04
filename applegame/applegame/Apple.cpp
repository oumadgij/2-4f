#include"Apple.h"
#include"common.h"
#include"Dxlib.h"
#include"Math.h"
#include"Player.h"
extern int g_Teki[3];
Apple apple[APPLE_MAX];

Apple::Apple() {
	flg = FALSE;	//使用フラグ
	type = 0;	//タイプ
	img = 0;	//画像
	x = 0;	//座標xy 幅w 高さh
	y = -50;
	w = 40;
	h = 40;
	speed = 0;	//速度
	point = 0;	//スコア加算
}

void Apple::Spawn(int x) {
	flg = TRUE;	//使用フラグ
	SelectApple();
	img = g_Teki[type];	//画像
	this->x = x * 62 + 62;	//座標xy 幅w 高さh
	y = -50;
	w = 40;
	h = 40;
}

void Apple::SelectApple(void) {
	int select = GetRand(99) + 1;
	if (select <= 60) {
		type = 0;
		speed = 2;
		point = 150;
	}
	else if (select <= 80) {
		type = 1;
		speed = 5;
		point = 300;
	}
	else if (select <= 90) {
		type = 2;
		speed = 10;
		point = 500;
	}
	else{
		type = 3;
		speed = 1;
		point = -1000;
	}
}

void Apple::EnemyControl() {
		if (flg == TRUE) {
			//敵の表示
			DrawGraph(x, y, img, TRUE);

			//真下に移動
			y += speed;

			//画面を出ると消滅
			if (y > SCREEN_HEIGHT + h)flg = FALSE;
		}
	
}

int Apple::GetType() {
	return type;
}
int Apple::GetX() {
	return x;
}
int Apple::GetY() {
	return y;
}
int Apple::GetWidth() {
	return w;
}
int Apple::GetHeight() {
	return h;
}