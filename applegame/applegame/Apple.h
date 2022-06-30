#pragma once
#include"common.h"

class Apple {
private:
	int type;	//タイプ
	int img;	//画像
	int x, y, w, h;	//座標xy 幅w 高さh
	int speed;	//速度
	int point;	//スコア加算

public:
	int flg;	//使用フラグ
	Apple();
	void Spawn(int x);			//リンゴ出現 x=出現するX座標の相対的な位置
	void EnemyControl();	//敵機処理
};

extern Apple apple[APPLE_MAX];