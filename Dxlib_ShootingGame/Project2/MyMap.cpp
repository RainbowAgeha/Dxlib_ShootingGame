#include "MyMap.h"

//�}�b�v�̑傫��

//�}�b�v�f�[�^
int MapDataNow[MAP_WIDTH][MAP_HEIGHT];
//�I�����Ă���}�b�v�f�[�^
int MapNo;


void MapInit() {
	//�}�b�v�̏�����
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

//�}�b�v�̓ǂݍ���(����:�t�@�C���p�X)
void MapLoad() {

}
//�}�b�v�̏�������(����:�t�@�C���p�X)
void MapSave() {

}