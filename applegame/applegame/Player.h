#pragma once
class Player {
private:
	float x;    //座標 x
	int y;		//座標 y
	int w, h;		//幅w 高さh
	int angle;	//機体の向き
	int RestD;		//りんごDの処理用
	float speed;	//移動速度
	float kansei;   //慣性

public:
	int flg;		//使用フラグ
	Player();
	void Init();
	void PlayerControl();	//自機処理
	void AppleColision(int i);	//リンゴの当たり判定
	int	 GetD();			//りんごDの処理
};

extern Player player;