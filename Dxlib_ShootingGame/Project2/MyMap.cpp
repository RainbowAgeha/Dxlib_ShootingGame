#include "MyMap.h"

//マップの大きさ

//マップデータ
int MapDataNow[MAP_WIDTH][MAP_HEIGHT];
//選択しているマップデータ
int MapNo;


void MapInit() {
	//マップの初期化
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			MapDataNow[i][j] = 0;
		}
	}


}
void MapSelectSet() {


}
void MapDraw() {

}

//マップの読み込み(引数:ファイルパス)
void MapLoad() {

}
//マップの書き込み(引数:ファイルパス)
void MapSave() {

}