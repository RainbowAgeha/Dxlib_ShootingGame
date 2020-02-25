
/*****

@Filename  : Game1Scene.cpp
@Comment   : シューティングゲームサンプルです。
			 １つのシーンでまとめて入っているので、別のシーンで使う際は
			 処理ごと分解してください。
			 TODO:やっている事
			 キャラクターのステータスをプレイヤー、敵、ボス、弾と全部に使用する
*****/

#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"

//計算関数定義
#include <math.h>

//時間取得の呼び出し
#include <time.h>

//外部定義(GameMain.cppにて宣言)
extern int Input, EdgeInput;

//数値定義---------------------

//キャラクターの数定義
#define CHARA_NUM 1000

enum CHARACTOR_ID {
	ID_NULL	= 0,
	ID_PLAYER,
	ID_BOSS,
	ID_ENEMY,
	ID_BULLET
};

#define CHARA_DEAD 0	//死んでいます
#define CHARA_ALIVE 1	//生きている

//ウィンドウサイズ
#define APP_WNDSX 640
#define APP_WNDSY 480
//シューティング画面サイズ
#define APP_CMW_MARGIN_X 120		//表示画面余白

//文字の色など
#define COLOR_WHITE 0xFFFFFF
#define COLOR_RED	0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE  0x0000FF
#define COLOR_GRAY  0x808080




//何でも使うよキャラクター構造体配列
struct CharacterStatus {
	int posx, posy;		//位置
	int Grafw, Grafh;	//画像の横幅
	int Hp, MaxHp;		//色々使うよHP
	int deadFlag;		//使用可どうか
	int charaID;		//キャラクター属性
	int propertyID;		//生成した持ち主(弾の持ち主など)
	int imgID;			//画像ID
	int AIid;			//動き方ID
	int Count;			//なにかに使うかもカウンター
};
//使う文だけ準備しておきましょう。
CharacterStatus charactor[CHARA_NUM];

enum GAMESTATUS_FLAG {
	GAME_INIT = 0,
	GAME_PLAY,
	GAME_BOSSEMPTY,
	GAME_CLEAR,
	GAME_OVER
};
//ゲーム管理
int GameStatus;
//色々使うカウンター
int EnemyRebornCounter;

int m_hFont;

//ユーザ関数定義

//簡易的な当たり判定
int hitcheck(int Ax1, int Ay1, int Aw, int Ah, int Bx1, int By1, int Bw, int Bh);

//構造体配列毎の当たり判定
int CharactorHitCheck(CharacterStatus *CharaA , CharacterStatus *CharaB);
int CharactorHitCheck(CharacterStatus* CharaA, int TargetID);

// シーン開始前の初期化を行う
BOOL initGame1Scene(void) {

	GameStatus = GAME_PLAY;

	//構造体配列の全初期化
	int i;
	for (i = 0; i < CHARA_NUM; i++) {
		charactor[i].posx = 0;
		charactor[i].posy = 0;	
		charactor[i].charaID = ID_NULL;		//誰のモノでもない
		charactor[i].propertyID = ID_NULL;		//誰のモノでもない
		charactor[i].deadFlag = CHARA_DEAD;	//生きていない
		charactor[i].Grafw = 32;		//仮のデフォルト値
		charactor[i].Grafh = 32;
		charactor[i].Hp = 0;
		charactor[i].MaxHp = 0;
		charactor[i].imgID = 0;		
		charactor[i].AIid = 0;		
		charactor[i].Count = 0;
	}

	//プレイヤーの生成
	for (i = 0; i < CHARA_NUM; i++) {
		if (charactor[i].deadFlag == CHARA_DEAD) {
			//多分添字0に生成されるはず。
			charactor[i].posx = APP_WNDSX / 2;
			charactor[i].posy = APP_WNDSY / 2 + APP_WNDSY / 4;	//画面下半分くらい
			charactor[i].charaID = ID_PLAYER;
			charactor[i].propertyID = ID_PLAYER;	//プレイヤーはプレイヤーのもの
			charactor[i].deadFlag = CHARA_ALIVE;	//使用するので生きているフラグに
			charactor[i].Grafw = 32;
			charactor[i].Grafh = 32;
			charactor[i].Hp = 10;
			charactor[i].MaxHp = 10;
			charactor[i].imgID = 0;		//今回は画像の読み込み無し
			charactor[i].AIid = 0;		//プレイヤーは使用しない
			charactor[i].Count = 0;
			break;
		}
	}

	//文字を生成する
	m_hFont = CreateFontToHandle("メイリオ", 36, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	return TRUE;
}

//	フレーム処理
void moveGame1Scene() {
	/*if ((EdgeInput & PAD_INPUT_1)) {
		changeScene(SCENE_MENU);
	}*/

	switch (GameStatus) {
	case GAME_INIT:
		break;
	case GAME_PLAY:
		//ゲームメイン処理----------------------------------

		//キャラクターを検索-----------------------
		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//生きているか検索
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;

			//プレイヤーの操作---------------------------------
			if (charactor[CharaI].charaID == ID_PLAYER){
				if (Input & PAD_INPUT_UP) {
					//キャラクター配列を検索
					charactor[CharaI].posy -= 5;
					if (charactor[CharaI].posy < 0) charactor[CharaI].posy = 0;
				}
				if (Input & PAD_INPUT_DOWN) {
					charactor[CharaI].posy += 5;
					if (charactor[CharaI].posy > APP_WNDSY - charactor[CharaI].Grafh) charactor[CharaI].posy = APP_WNDSY - charactor[CharaI].Grafh;
				}
				if (Input & PAD_INPUT_LEFT) {
					charactor[CharaI].posx -= 5;
					if (charactor[CharaI].posx < 0) charactor[CharaI].posx = 0;
				}
				if (Input & PAD_INPUT_RIGHT) {
					charactor[CharaI].posx += 5;
					if (charactor[CharaI].posx > APP_WNDSX - charactor[CharaI].Grafw) charactor[CharaI].posx = APP_WNDSX - charactor[CharaI].Grafw;
				}

				//ボタン1で弾を発射
				if (EdgeInput & PAD_INPUT_1) {
					//使ってない弾を検索
					for (int bulleti = 0; bulleti < CHARA_NUM; bulleti++) {
						if (charactor[bulleti].deadFlag == CHARA_DEAD) {
							//弾を生成する
							charactor[bulleti].posx = charactor[CharaI].posx;
							charactor[bulleti].posy = charactor[CharaI].posy;
							charactor[bulleti].deadFlag = CHARA_ALIVE;
							charactor[bulleti].charaID = ID_BULLET;
							charactor[bulleti].propertyID = ID_PLAYER;	//プレイヤーから生成
							charactor[bulleti].AIid = 1;		//とりあえず上に飛ばす
							break;	//生成したのでfor文を終了する
						}
					}
				}

				//ボタン4で敵を生成
				if (EdgeInput & PAD_INPUT_4) {
					//使ってない弾を検索
					for (int enemyi = 0; enemyi < CHARA_NUM; enemyi++) {
						if (charactor[enemyi].deadFlag == CHARA_DEAD) {
							//弾を生成する
							charactor[enemyi].posx = charactor[CharaI].posx;
							//charactor[enemyi].posy = -120;		//範囲外より内側に
							charactor[enemyi].posy = 40;	//
							charactor[enemyi].deadFlag = CHARA_ALIVE;
							charactor[enemyi].charaID = ID_ENEMY;
							charactor[enemyi].propertyID = ID_ENEMY;
							charactor[enemyi].AIid = 0;		//とりあえず上に飛ばす
							break;	//生成したのでfor文を終了する
						}
					}
				}

			}
			//プレイヤーの操作---------------------------------

			//敵の操作---------------------------------
			if (charactor[CharaI].charaID == ID_ENEMY) {
				switch (charactor[CharaI].AIid) {
				case 0:
					break;
				case 1:		//基本的な下に移動するのみ
					charactor[CharaI].posy += 5;
					break;
				case 2:
					break;
				}

				//画面外に出たら消える(敵なので、画面外より少し遠くに
				if (charactor[CharaI].posx < -150 || charactor[CharaI].posx > APP_WNDSX + 150 ||
					charactor[CharaI].posy < -150 || charactor[CharaI].posy > APP_WNDSY + 150)
					charactor[CharaI].deadFlag = CHARA_DEAD;
			}
			//敵の操作---------------------------------

			//弾の操作---------------------------------
			if (charactor[CharaI].charaID == ID_BULLET) {
				switch (charactor[CharaI].AIid) {
				case 0:
					break;
				case 1:		//基本的な上に移動するのみ
					charactor[CharaI].posy -= 5;
					break;
				case 2:
					charactor[CharaI].posy += 5;
					break;
				}

				//画面外に出たら消える(弾なので、画面外より少し遠くに
				if (charactor[CharaI].posx < -150 || charactor[CharaI].posx > APP_WNDSX + 150 ||
					charactor[CharaI].posy < -150 || charactor[CharaI].posy > APP_WNDSY + 150)
					charactor[CharaI].deadFlag = CHARA_DEAD;

				//プレイヤーの弾と、敵との当たり判定を行う
				if (charactor[CharaI].propertyID == ID_PLAYER) {
					CharactorHitCheck(&charactor[CharaI], ID_ENEMY);
				}

			}
			//弾の操作---------------------------------
		}
		//キャラクターを検索-----------------------


		//ゲームメイン処理----------------------------------
		break;
	case GAME_BOSSEMPTY:
		//ボスモード処理------------------------------------

		//ボスモード処理------------------------------------
		break;
	case GAME_CLEAR:
	case GAME_OVER:
		//ゲームクリアもオーバーも、とりあえずタイトルへ
		if ((EdgeInput & PAD_INPUT_1)) {
			changeScene(SCENE_MENU);
		}
		break;

	}

}


//	レンダリング処理
void renderGame1Scene(void){

	float mojix, mojiy;	//文字位置
	int i;		//配列とかに使うやつ

	//処理の分岐点
	switch (GameStatus) {
	case GAME_INIT:
		break;
	case GAME_PLAY:
		//ゲームメイン処理----------------------------------

		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//生きているか検索
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
			if (charactor[CharaI].charaID == ID_PLAYER) {
				//プレイヤーの表示(仮のボックス表示)
				DrawBox(charactor[CharaI].posx, charactor[CharaI].posy,
					charactor[CharaI].posx + charactor[CharaI].Grafw,
					charactor[CharaI].posy + charactor[CharaI].Grafh,
					COLOR_RED, true);
			}

			if (charactor[CharaI].charaID == ID_ENEMY) {
				DrawBox(charactor[CharaI].posx, charactor[CharaI].posy,
					charactor[CharaI].posx + charactor[CharaI].Grafw,
					charactor[CharaI].posy + charactor[CharaI].Grafh,
					COLOR_BLUE, true);
			}

			if (charactor[CharaI].charaID == ID_BULLET) {
				DrawBox(charactor[CharaI].posx, charactor[CharaI].posy,
					charactor[CharaI].posx + charactor[CharaI].Grafw,
					charactor[CharaI].posy + charactor[CharaI].Grafh,
					COLOR_GREEN, true);
			}
		}

	case GAME_BOSSEMPTY:	//ボスモードのみの表示
		//ヒットポイントの表示しましょ
	
		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//生きているか検索
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
			if (charactor[CharaI].charaID == ID_BOSS) {

			}
		}
		////下の赤いゲージ(最大HP)
		//DrawBox(charactor[CharaI].posx, charactor[CharaI].posy,
		//	charactor[CharaI].posx + charactor[CharaI].Grafw,
		//	charactor[CharaI].posy + charactor[CharaI].Grafh,
		//	COLOR_GREEN, true);
		//上の緑ゲージ(現在HP)
		break;
		//ゲームメイン処理----------------------------------

	case GAME_CLEAR:
		//文字を画面中央におく(画面の半分と、文字数からなる長さの半分を計算)
		mojix = APP_WNDSX / 2 - GetDrawStringWidthToHandle("ゲームクリア", strlen("ゲームクリア"), m_hFont) / 2;
		mojiy = APP_WNDSY / 2 - GetFontSizeToHandle(m_hFont)/2;
		DrawExtendStringToHandle(mojix, mojiy, 1, 1, "ゲームクリア", COLOR_WHITE, m_hFont, COLOR_GRAY);
		break;
	case GAME_OVER:
		mojix = APP_WNDSX / 2 - GetDrawStringWidthToHandle("ゲームオーバー", strlen("ゲームオーバー"), m_hFont) / 2;
		mojiy = APP_WNDSY / 2 - GetFontSizeToHandle(m_hFont) / 2;
		DrawExtendStringToHandle(mojix, mojiy, 1, 1, "ゲームオーバー", COLOR_WHITE, m_hFont, COLOR_GRAY);

		break;

	}
	
}

//	シーン終了時の後処理
void releaseGame1Scene(void) {


}

// 当り判定コールバック 　　　ここでは要素を削除しないこと！！
void  Game1SceneCollideCallback(int nSrc, int nTarget, int nCollideID) {

}


//当たり判定
int hitcheck(int Ax1, int Ay1, int Aw, int Ah, int Bx1, int By1, int Bw, int Bh) {
	int Ax2, Ay2, Bx2, By2;
	Ax2 = Ax1 + Aw;
	Ay2 = Ay1 + Ah;
	Bx2 = Bx1 + Bw;
	By2 = By1 + Bh;

	if (Ax1 < Bx2 && Bx1 < Ax2 && Ay1 < By2 && By1 < Ay2) {
		return 1;
	}
	else {
		return 0;
	}
}


//構造体配列との当たり判定(当たった場合添字番号を返す ,当たらなかった-1)
int CharactorHitCheck(CharacterStatus* CharaA, CharacterStatus* CharaB) {

	int Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2;
	Ax1 = CharaA->posx;
	Ay1 = CharaA->posy;
	Ax2 = Ax1 + CharaA->Grafw;
	Ay2 = Ay1 + CharaA->Grafh;

	Bx1 = CharaB->posx;
	By1 = CharaB->posy;
	Bx2 = Bx1 + CharaB->Grafw;
	By2 = By1 + CharaB->Grafh;

	//どれかと当たったら当たったフラグを返す
	if (Ax1 < Bx2 && Bx1 < Ax2 && Ay1 < By2 && By1 < Ay2) {
		return 1;
	}
	
	//どれとも当たらなかったら0を返す
	return -1;
	
}

//使用している構造体配列と、当たり判定のターゲットを指定し、当たっているか判定する
//TODO:もっと良いやり方ありそうだけど暫定
int CharactorHitCheck(CharacterStatus* CharaA,int TargetID) {
	//自分のIDを取得
	int YourID = CharaA->charaID;

	//構造体配列を検索
	for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
		//死んでいるものは無視する
		if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
		//自分自身だった場合も無視する(自分と同じグループとの当たり判定って基本ないよね？
		if (charactor[CharaI].charaID == YourID)continue;
		//自分の持ち物だった場合も無視する
		if (charactor[CharaI].charaID == CharaA->propertyID)continue;

		//検索するIDだったら、当たり判定を行う
		if (charactor[CharaI].charaID == TargetID) {
			int Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2;
			Ax1 = CharaA->posx;
			Ay1 = CharaA->posy;
			Ax2 = Ax1 + CharaA->Grafw;
			Ay2 = Ay1 + CharaA->Grafh;

			Bx1 = charactor[CharaI].posx;
			By1 = charactor[CharaI].posy;
			Bx2 = Bx1 + charactor[CharaI].Grafw;
			By2 = By1 + charactor[CharaI].Grafh;

			//どれかと当たったら当たったフラグを返す
			if (Ax1 < Bx2 && Bx1 < Ax2 && Ay1 < By2 && By1 < Ay2) {
				//コイツラが当たったので、死ぬ命令を下す
				CharaA->deadFlag = CHARA_DEAD;
				charactor[CharaI].deadFlag = CHARA_DEAD;
				return 1;
			}
		}
	}

	//検索終わったけど返っていない＝当たらなかった。
	return 0;

}