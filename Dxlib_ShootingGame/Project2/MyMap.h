#pragma once

//マップの大きさ
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 7

#define BLOCKSIZE 32


void MapInit();
void MapSelectSet();
void MapDraw();

//マップの読み込み(引数:ファイルパス)
void MapLoad();
//マップの書き込み(引数:ファイルパス)
void MapSave();