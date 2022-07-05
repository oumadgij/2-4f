#include"DxLib.h"
#include"common.h"
#include"Player.h"
#include"Apple.h"
#include <math.h>

//変数の宣言
int g_OldKey;			//前回の入力キー
int g_NowKey;			//今回の入力キー
int g_KeyFlg;			//入力キー情報

int g_GameState = 0;	//ゲームモード
int g_WaitTime;		//待ち時間

int g_Score;			//スコア
int g_EnemyCount1;		//敵カウント
int g_EnemyCount2;
int g_EnemyCount3;
int g_EnemyCount4;

int SpawnX[7] = { 0,0,0,0,0,0,0 };			//リンゴ生成位置(0:未生成 1:生成済み)

int g_TitleImage;		//画像用変数
int g_Menu, g_Cone;		//メニュー画像変数
int g_RankingImage;		//ランキング画像用変数
int g_Item[2];			//アイテム画像変数
int g_EndImage;			//エンド画面用変数
int g_Teki[4];			//敵画像変数
int g_StageImage;		//ステージ画像変数
int g_PlayerImage[2];	//キャラ画像

int StartKeyFlg;
double NextTime;		//フレーム毎の経過時間

//自機初期値

//ランキングデータ(構造体)
struct RankingData {
	int no;
	char name[11];
	long score;
};

//ランキングデータ変数宣言
struct RankingData	g_Ranking[RANKING_DATA];

//関数のプロトタイプ宣言
void GameInit(void);	//ゲーム初期化処理
void GameMain(void);	//ゲームメイン処理

void DrawGameTitle(void);	//タイトル描画処理
void DrawGameOver(void);	//ゲームオーバー画面描画処理
void DrawEnd(void);			//ゲームエンド描画処理
void DrawHelp(void);		//ゲームヘルプ描画処理

int CheckApple(void);		//リンゴの生成可能数チェック
int SpawnAppleX(void);		//出現するリンゴのX座標を決定

void BackScrool();		//背景画像スクロール処理

//int HitBoxPlayer(PLAYER *p, ENEMY *e);	//当たり判定

void DrawRanking(void);		//ランキング描画処理
void InputRanking(void);	//ランキング入力
void SortRanking(void);		//ランキング並び替え
int  SaveRanking(void);		//ランキングデータの保存
int  ReadRanking(void);		//ランキングデータ読み込み

int LoadImages();			//画像読み込み

void CheckPauseKey(void);	//ポーズ　startKeyのチェック

//プログラムの開始
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("DriveGame");		//タイトルを設定

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	if (LoadImages() == -1)return -1;

	if (ReadRanking() == -1)return -1;

	while (ProcessMessage() == 0 && g_GameState != 99 && !(g_KeyFlg & PAD_INPUT_M)) {
	
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();	//画面の初期化

		switch (g_GameState) {
		case 0:
			DrawGameTitle();	//ゲームタイトル描画
			break;

		case 1:
			GameInit();			//ゲーム初期処理
			break;

		case 2:
			DrawRanking();		//ランキング描画
			break;

		case 3:
			DrawHelp();			//ゲームヘルプ描画
			break;

		case 4:
			DrawEnd();			//ゲームエンド描画
			break;

		case 5:
			GameMain();			//ゲームメイン処理
			break;

		case 6:
			DrawGameOver();		//ゲームオーバー描画
			break;

		case 7:
			InputRanking();		//ランキング入力
			break;
		}

		NextTime += 16.66;								//1フレームごとに 1000/60 = 約16.66ミリ秒かかる			
		if (NextTime > GetNowCount()) {					//経過フレーム*16.66秒経過した？
			WaitTimer((int)NextTime - GetNowCount());	//してないなら経過するまで待つ
		}
		ScreenFlip();			//裏画面の内容を表画面に反映
	}
	DxLib_End();				//DXライブラリ仕様の終了処理

	return 0;					//ソフトの終了

}

//ゲームタイトル表示
void DrawGameTitle(void) {
	static int MenuNo = 0;

	if (MenuNo > 3 || MenuNo < 0) MenuNo = 0;

	
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 3)MenuNo = 0;
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 3;
	}

	//Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_A)g_GameState = MenuNo + 1;

	//タイトル画像表示
	DrawGraph(0, 0, g_TitleImage, FALSE);

	//メニュー
	DrawGraph(120, 200, g_Menu, TRUE);

	//メニューカーソル
	DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, 0, g_Cone, TRUE);
}

//ゲーム初期処理
void GameInit(void) {

	//時間初期化
	g_WaitTime = 0;

	//スコア初期化
	g_Score = 0;

	//敵の初期設定
	g_EnemyCount1 = 0;
	g_EnemyCount2 = 0;
	g_EnemyCount3 = 0;
	g_EnemyCount4 = 0;

	//プレイヤー初期設定
	player.Init();

	//敵の初期設定
	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].flg = FALSE;
	}

	//ゲームメイン処理へ
	g_GameState = 5;
}

//ゲームランキング画面表示
void DrawRanking(void) {

	//スペースキーでメニューに戻る
	if (g_KeyFlg & PAD_INPUT_M)g_GameState = 0;

	//ランキング画面表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	//ランキング一覧を表示
	SetFontSize(18);
	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	DrawString(100, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);

}

//ゲームヘルプ描画
void DrawHelp(void) {

	//スペースキーでメニューに戻る
	if (g_KeyFlg & PAD_INPUT_M)g_GameState = 0;

	//タイトル画像表示
	DrawGraph(0, 0, g_TitleImage, FALSE);
	SetFontSize(16);
	DrawString(20, 120, "- HELP -", 0xffffff, 0);

	DrawString(20, 160, "障害物を避けながら", 0xffffff, 0);
	DrawString(20, 180, "走り続けよう", 0xffffff, 0);
	DrawString(20, 200, "燃料が尽きるか", 0xffffff, 0);
	DrawString(20, 220, "障害物に数回当たるとゲームオーバー", 0xffffff, 0);
	DrawString(20, 250, "アイテム一覧", 0xffffff, 0);
	DrawGraph(20, 260, g_Item[0], TRUE);
	DrawString(20, 315, "燃料を回復できる", 0xffffff, 0);
	DrawGraph(20, 335, g_Item[1], TRUE);
	DrawString(20, 385, "耐久を回復できる", 0xffffff, 0);
	DrawString(20, 405, "無傷なら燃料を少し回復できる", 0xffffff, 0);
	DrawString(150, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);

}

//ゲームエンド描画
void DrawEnd(void) {

	//エンド画面表示
	DrawGraph(0, 0, g_EndImage, FALSE);

	SetFontSize(24);
	DrawString(360, 480 - 24, "Thankyou for Playing", 0xffffff, 0);

	//タイム加算処理&終了(3秒)
	if (++g_WaitTime > 180)g_GameState = 99;

}

//ゲームメイン
void GameMain(void) {

	static int restapple = CheckApple();	//リンゴ生成可能数

	BackScrool();

	
	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].EnemyControl();	//リンゴ制御
		if (player.GetD() <= 0) {		//りんごDの効果中ではない？
			player.AppleColision(i);	//効果中でないならりんごの当たり判定を取る
		}
	}
	if (g_WaitTime++ % 25 == 0&& g_WaitTime % 25 != 0) {	//25fごとにリンゴ出現
		for (int i = 0; i < (restapple + 1) / 2; i++) {		//リンゴの生成数チェック(生成可能数/2)
			for (int j = i; j < APPLE_MAX; j++) {			
				if (apple[j].flg == FALSE) {			//apple[j]のりんごが出現中か(フラグがTRUEか)
					apple[j].Spawn(SpawnAppleX());		//非出現(フラグがFALSE)ならX座標を決定しリンゴを出現させる
					j += APPLE_MAX;
				}
			}
		}
		for (int k = 0; k < 7; k++) {
			SpawnX[k] = 0;				//リンゴ生成位置をリセット
		}
		restapple = CheckApple();
	}

	player.PlayerControl();	//プレイヤー制御
	CheckPauseKey();	//ポーズ


}

//リンゴの生成可能数チェック
int CheckApple(void) {
	int rest = 0;

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].flg == FALSE) rest++;
	}
	return rest;
}

//出現するリンゴのX座標を決定
int SpawnAppleX(void) {
	int x = 0;

	do  {
		x = GetRand(6);			//X座標をランダム選択
	} while (SpawnX[x] != 0);	//その位置にすでに生成されている？

	SpawnX[x] = 1;
	return x;
}

//背景画像スクロール処理
void BackScrool() {

	//ステージ画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, 500, 480);

	DrawGraph(0, 0, g_StageImage, FALSE);

	//エリアを戻す
	SetDrawArea(0, 0, 640, 480);

	//スコア等表示領域
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);
}

//敵との当たり判定
//int HitBoxPlayer(player *p, apple *e) {
//	//x,yは中心座標
//	int sx1 = p->x - (p->w / 2) + 5;
//	int sy1 = p->y - (p->h / 2) + 5;
//	int sx2 = sx1 + p->w - 10;
//	int sy2 = sy1 + p->h - 10;
//
//	int dx1 = e->x - (e->w / 2) + 5;
//	int dy1 = e->y - (e->h / 2) + 5;
//	int dx2 = dx1 + e->w - 10;
//	int dy2 = dy1 + e->h - 10;
//
//	//矩形が重なれば当たり
//	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
//		return TRUE;
//	}
//	return FALSE;
//}
// 
//ゲームオーバー画面描画

void DrawGameOver(void) {

	BackScrool();

	//スペースキーでメニューに戻る
	if (g_KeyFlg & PAD_INPUT_M) {
		if (g_Ranking[RANKING_DATA-1].score >= g_Score) {
			g_GameState = 0;
		}
		else {
			g_GameState = 7;
		}
	}

	DrawString(150, 450, "---- PRESS SPACE KEY TO TITLE ----", 0xffffff, 0);
}

 //ランキング入力処理
void InputRanking(void)
{
	//ランキング画像表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	// フォントサイズの設定
	SetFontSize(20);

	// 名前入力指示文字列の描画
	DrawString(150, 240, "ランキングに登録します", 0xFFFFFF);
	DrawString(150, 270, "名前を英字で入力してください", 0xFFFFFF);

	// 名前の入力
	DrawString(150, 310, "> ", 0xFFFFFF);
	DrawBox(160, 305, 300, 335, 0x000055, TRUE);
	if (KeyInputSingleCharString(170, 310, 10, g_Ranking[4].name, FALSE) == 1) {
		g_Ranking[4].score = g_Score;	// ランキングデータの5番目にスコアを登録
		SortRanking();		// ランキング並べ替え
		SaveRanking();		// ランキングデータの保存
		g_GameState = 2;		// ゲームモードの変更
	}

}

 //ランキング並べ替え
void SortRanking(void)
{
	int i, j;
	RankingData work;

	// 選択法ソート
	for (i = 0; i < 9; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// 順位付け
	for (i = 0; i < 10; i++) {
		g_Ranking[i].no = 1;
	}
	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (i = 0; i < 9; i++) {
		for (j = i + 1; j < 10; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}

 //ランキングデータの保存
int  SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* エラー処理 */
		printf("Ranking Data Error\n");
		return -1;
	}

	// ランキングデータ分配列データを書き込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;

}

 //ランキングデータ読み込み
int ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fscanf(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;
}

int LoadImages() {

	//タイトル
	if ((g_TitleImage = LoadGraph("images/Title.bmp")) == -1)return -1;

	//メニュー
	if ((g_Menu = LoadGraph("images/menu.bmp")) == -1)return -1;
	if ((g_Cone = LoadGraph("images/leaf.png")) == -1)return -1;

	//ランキング
	if ((g_RankingImage = LoadGraph("images/Ranking.bmp")) == -1)return -1;

	//ステージ背景
	if ((g_StageImage = LoadGraph("images/back.png")) == -1)return -1;

	//プレイヤー
	if ((g_PlayerImage[0] = LoadGraph("images/player0.png")) == -1)return -1;
	if ((g_PlayerImage[1] = LoadGraph("images/player1.png")) == -1)return -1;

	//敵
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 40, 40, g_Teki) == -1)return -1;

	//エンディング
	if ((g_EndImage = LoadGraph("images/End.bmp")) == -1)return -1;

	return 0;
}

void CheckPauseKey(void) {
	if (g_KeyFlg & PAD_INPUT_8)		//指定キーでflgを1
	{
		int flg = 1;

		while (ProcessMessage() == 0 && flg)
		{
			g_OldKey = g_NowKey;
			g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
			g_KeyFlg = g_NowKey & ~g_OldKey;


			SetFontSize(46);
			DrawString(180, 200, "Xx-POSE-xX", GetColor(0, 0, 0), 1);


			if (g_KeyFlg & PAD_INPUT_8)flg = 0;		//指定キーでFlgを0

			ScreenFlip();			//裏画面の内容を表画面に反映

		}
	}
}