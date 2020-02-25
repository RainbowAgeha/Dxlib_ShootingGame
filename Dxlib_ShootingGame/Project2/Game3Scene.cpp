#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game3Scene.h"

//数学処理を行う際の読み込み処理
#include "math.h"

//外部定義(GameMain.cppにて宣言)
extern int Input, EdgeInput;


//ユーザー関数宣言
int hitcheck(int Ax1, int Ay1, int Aw, int Ah, int Bx1, int By1, int Bw, int Bh);


// シーン開始前の初期化を行う
BOOL initGame3Scene(void){


	return TRUE;
}

//	フレーム処理
void moveGame3Scene(){

	
}

//	レンダリング処理
void renderGame3Scene(void){


}

//	シーン終了時の後処理
void releaseGame3Scene(void){


}

// 当り判定コールバック 　　　ここでは要素を削除しないこと！！
void  Game3SceneCollideCallback( int nSrc, int nTarget, int nCollideID ){



}
