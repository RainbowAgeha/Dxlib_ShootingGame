#pragma once

//�}�b�v�̑傫��
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 7

#define BLOCKSIZE 32


void MapInit();
void MapSelectSet();
void MapDraw();

//�}�b�v�̓ǂݍ���(����:�t�@�C���p�X)
void MapLoad();
//�}�b�v�̏�������(����:�t�@�C���p�X)
void MapSave();