#pragma once
class Player {
private:
	int x, y;		//座標x y
	int w, h;		//幅w 高さh
	double angle;	//機体の向き
	int count;		//タイミング用
	int speed;		//移動速度

public:
	int flg;		//使用フラグ
	Player();
	void Init();
	void PlayerControl();	//自機処理
};

extern Player player;