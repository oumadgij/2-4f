#pragma once
class Player {
private:
	int x, y;		//座標x y
	int w, h;		//幅w 高さh
	double angle;	//機体の向き
	int count;		//タイミング用
	int speed;		//移動速度
	int hp;			//体力
	int fuel;		//燃料
	int bari;		//バリア回数
	int baricnt;	//バリア継続時間
	int bariup;		//バリア回数(走行距離アップ)

public:
	int flg;		//使用フラグ
	Player();
	void Init();
	void PlayerControl();	//自機処理
};

extern Player player;