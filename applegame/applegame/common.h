#pragma once
#define	_USE_MATH_DEFINES
#define	RANKING_DATA	5

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int APPLE_MAX = 10;

const int TIMELIMIT = 30000;

extern int g_StartTime;		//スタート時間
extern int g_WaitTime;		//待ち時間
extern int g_GameState;		// ゲームステータス

extern int g_OldKey;			//前回の入力キー
extern int g_NowKey;			//今回の入力キー
extern int g_KeyFlg;			//入力キー情報

extern int g_Score;			//スコア
extern int g_EnemyCount1;		//敵カウント
extern int g_EnemyCount2;
extern int g_EnemyCount3;
extern int g_EnemyCount4;

extern int FontHandle;