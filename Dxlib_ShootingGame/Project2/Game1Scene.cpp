
/*****

@Filename  : Game1Scene.cpp
@Comment   : �V���[�e�B���O�Q�[���T���v���ł��B
			 �P�̃V�[���ł܂Ƃ߂ē����Ă���̂ŁA�ʂ̃V�[���Ŏg���ۂ�
			 �������ƕ������Ă��������B
			 TODO:����Ă��鎖
			 �L�����N�^�[�̃X�e�[�^�X���v���C���[�A�G�A�{�X�A�e�ƑS���Ɏg�p����
*****/

#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"

//�v�Z�֐���`
#include <math.h>

//���Ԏ擾�̌Ăяo��
#include <time.h>

//�O����`(GameMain.cpp�ɂĐ錾)
extern int Input, EdgeInput;

//���l��`---------------------

//�L�����N�^�[�̐���`
#define CHARA_NUM 1000

enum CHARACTOR_ID {
	ID_NULL	= 0,
	ID_PLAYER,
	ID_BOSS,
	ID_ENEMY,
	ID_BULLET
};

#define CHARA_DEAD 0	//����ł��܂�
#define CHARA_ALIVE 1	//�����Ă���

//�E�B���h�E�T�C�Y
#define APP_WNDSX 640
#define APP_WNDSY 480
//�V���[�e�B���O��ʃT�C�Y
#define APP_CMW_MARGIN_X 120		//�\����ʗ]��

//�����̐F�Ȃ�
#define COLOR_WHITE 0xFFFFFF
#define COLOR_RED	0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE  0x0000FF
#define COLOR_GRAY  0x808080




//���ł��g����L�����N�^�[�\���̔z��
struct CharacterStatus {
	int posx, posy;		//�ʒu
	int Grafw, Grafh;	//�摜�̉���
	int Hp, MaxHp;		//�F�X�g����HP
	int deadFlag;		//�g�p�ǂ���
	int charaID;		//�L�����N�^�[����
	int propertyID;		//��������������(�e�̎�����Ȃ�)
	int imgID;			//�摜ID
	int AIid;			//������ID
	int Count;			//�Ȃɂ��Ɏg�������J�E���^�[
};
//�g���������������Ă����܂��傤�B
CharacterStatus charactor[CHARA_NUM];

enum GAMESTATUS_FLAG {
	GAME_INIT = 0,
	GAME_PLAY,
	GAME_BOSSEMPTY,
	GAME_CLEAR,
	GAME_OVER
};
//�Q�[���Ǘ�
int GameStatus;
//�F�X�g���J�E���^�[
int EnemyRebornCounter;

int m_hFont;

//���[�U�֐���`

//�ȈՓI�ȓ����蔻��
int hitcheck(int Ax1, int Ay1, int Aw, int Ah, int Bx1, int By1, int Bw, int Bh);

//�\���̔z�񖈂̓����蔻��
int CharactorHitCheck(CharacterStatus *CharaA , CharacterStatus *CharaB);
int CharactorHitCheck(CharacterStatus* CharaA, int TargetID);

// �V�[���J�n�O�̏��������s��
BOOL initGame1Scene(void) {

	GameStatus = GAME_PLAY;

	//�\���̔z��̑S������
	int i;
	for (i = 0; i < CHARA_NUM; i++) {
		charactor[i].posx = 0;
		charactor[i].posy = 0;	
		charactor[i].charaID = ID_NULL;		//�N�̃��m�ł��Ȃ�
		charactor[i].propertyID = ID_NULL;		//�N�̃��m�ł��Ȃ�
		charactor[i].deadFlag = CHARA_DEAD;	//�����Ă��Ȃ�
		charactor[i].Grafw = 32;		//���̃f�t�H���g�l
		charactor[i].Grafh = 32;
		charactor[i].Hp = 0;
		charactor[i].MaxHp = 0;
		charactor[i].imgID = 0;		
		charactor[i].AIid = 0;		
		charactor[i].Count = 0;
	}

	//�v���C���[�̐���
	for (i = 0; i < CHARA_NUM; i++) {
		if (charactor[i].deadFlag == CHARA_DEAD) {
			//�����Y��0�ɐ��������͂��B
			charactor[i].posx = APP_WNDSX / 2;
			charactor[i].posy = APP_WNDSY / 2 + APP_WNDSY / 4;	//��ʉ��������炢
			charactor[i].charaID = ID_PLAYER;
			charactor[i].propertyID = ID_PLAYER;	//�v���C���[�̓v���C���[�̂���
			charactor[i].deadFlag = CHARA_ALIVE;	//�g�p����̂Ő����Ă���t���O��
			charactor[i].Grafw = 32;
			charactor[i].Grafh = 32;
			charactor[i].Hp = 10;
			charactor[i].MaxHp = 10;
			charactor[i].imgID = 0;		//����͉摜�̓ǂݍ��ݖ���
			charactor[i].AIid = 0;		//�v���C���[�͎g�p���Ȃ�
			charactor[i].Count = 0;
			break;
		}
	}

	//�����𐶐�����
	m_hFont = CreateFontToHandle("���C���I", 36, 6, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

	return TRUE;
}

//	�t���[������
void moveGame1Scene() {
	/*if ((EdgeInput & PAD_INPUT_1)) {
		changeScene(SCENE_MENU);
	}*/

	switch (GameStatus) {
	case GAME_INIT:
		break;
	case GAME_PLAY:
		//�Q�[�����C������----------------------------------

		//�L�����N�^�[������-----------------------
		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//�����Ă��邩����
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;

			//�v���C���[�̑���---------------------------------
			if (charactor[CharaI].charaID == ID_PLAYER){
				if (Input & PAD_INPUT_UP) {
					//�L�����N�^�[�z�������
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

				//�{�^��1�Œe�𔭎�
				if (EdgeInput & PAD_INPUT_1) {
					//�g���ĂȂ��e������
					for (int bulleti = 0; bulleti < CHARA_NUM; bulleti++) {
						if (charactor[bulleti].deadFlag == CHARA_DEAD) {
							//�e�𐶐�����
							charactor[bulleti].posx = charactor[CharaI].posx;
							charactor[bulleti].posy = charactor[CharaI].posy;
							charactor[bulleti].deadFlag = CHARA_ALIVE;
							charactor[bulleti].charaID = ID_BULLET;
							charactor[bulleti].propertyID = ID_PLAYER;	//�v���C���[���琶��
							charactor[bulleti].AIid = 1;		//�Ƃ肠������ɔ�΂�
							break;	//���������̂�for�����I������
						}
					}
				}

				//�{�^��4�œG�𐶐�
				if (EdgeInput & PAD_INPUT_4) {
					//�g���ĂȂ��e������
					for (int enemyi = 0; enemyi < CHARA_NUM; enemyi++) {
						if (charactor[enemyi].deadFlag == CHARA_DEAD) {
							//�e�𐶐�����
							charactor[enemyi].posx = charactor[CharaI].posx;
							//charactor[enemyi].posy = -120;		//�͈͊O��������
							charactor[enemyi].posy = 40;	//
							charactor[enemyi].deadFlag = CHARA_ALIVE;
							charactor[enemyi].charaID = ID_ENEMY;
							charactor[enemyi].propertyID = ID_ENEMY;
							charactor[enemyi].AIid = 0;		//�Ƃ肠������ɔ�΂�
							break;	//���������̂�for�����I������
						}
					}
				}

			}
			//�v���C���[�̑���---------------------------------

			//�G�̑���---------------------------------
			if (charactor[CharaI].charaID == ID_ENEMY) {
				switch (charactor[CharaI].AIid) {
				case 0:
					break;
				case 1:		//��{�I�ȉ��Ɉړ�����̂�
					charactor[CharaI].posy += 5;
					break;
				case 2:
					break;
				}

				//��ʊO�ɏo���������(�G�Ȃ̂ŁA��ʊO��菭��������
				if (charactor[CharaI].posx < -150 || charactor[CharaI].posx > APP_WNDSX + 150 ||
					charactor[CharaI].posy < -150 || charactor[CharaI].posy > APP_WNDSY + 150)
					charactor[CharaI].deadFlag = CHARA_DEAD;
			}
			//�G�̑���---------------------------------

			//�e�̑���---------------------------------
			if (charactor[CharaI].charaID == ID_BULLET) {
				switch (charactor[CharaI].AIid) {
				case 0:
					break;
				case 1:		//��{�I�ȏ�Ɉړ�����̂�
					charactor[CharaI].posy -= 5;
					break;
				case 2:
					charactor[CharaI].posy += 5;
					break;
				}

				//��ʊO�ɏo���������(�e�Ȃ̂ŁA��ʊO��菭��������
				if (charactor[CharaI].posx < -150 || charactor[CharaI].posx > APP_WNDSX + 150 ||
					charactor[CharaI].posy < -150 || charactor[CharaI].posy > APP_WNDSY + 150)
					charactor[CharaI].deadFlag = CHARA_DEAD;

				//�v���C���[�̒e�ƁA�G�Ƃ̓����蔻����s��
				if (charactor[CharaI].propertyID == ID_PLAYER) {
					CharactorHitCheck(&charactor[CharaI], ID_ENEMY);
				}

			}
			//�e�̑���---------------------------------
		}
		//�L�����N�^�[������-----------------------


		//�Q�[�����C������----------------------------------
		break;
	case GAME_BOSSEMPTY:
		//�{�X���[�h����------------------------------------

		//�{�X���[�h����------------------------------------
		break;
	case GAME_CLEAR:
	case GAME_OVER:
		//�Q�[���N���A���I�[�o�[���A�Ƃ肠�����^�C�g����
		if ((EdgeInput & PAD_INPUT_1)) {
			changeScene(SCENE_MENU);
		}
		break;

	}

}


//	�����_�����O����
void renderGame1Scene(void){

	float mojix, mojiy;	//�����ʒu
	int i;		//�z��Ƃ��Ɏg�����

	//�����̕���_
	switch (GameStatus) {
	case GAME_INIT:
		break;
	case GAME_PLAY:
		//�Q�[�����C������----------------------------------

		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//�����Ă��邩����
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
			if (charactor[CharaI].charaID == ID_PLAYER) {
				//�v���C���[�̕\��(���̃{�b�N�X�\��)
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

	case GAME_BOSSEMPTY:	//�{�X���[�h�݂̂̕\��
		//�q�b�g�|�C���g�̕\�����܂���
	
		for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
			//�����Ă��邩����
			if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
			if (charactor[CharaI].charaID == ID_BOSS) {

			}
		}
		////���̐Ԃ��Q�[�W(�ő�HP)
		//DrawBox(charactor[CharaI].posx, charactor[CharaI].posy,
		//	charactor[CharaI].posx + charactor[CharaI].Grafw,
		//	charactor[CharaI].posy + charactor[CharaI].Grafh,
		//	COLOR_GREEN, true);
		//��̗΃Q�[�W(����HP)
		break;
		//�Q�[�����C������----------------------------------

	case GAME_CLEAR:
		//��������ʒ����ɂ���(��ʂ̔����ƁA����������Ȃ钷���̔������v�Z)
		mojix = APP_WNDSX / 2 - GetDrawStringWidthToHandle("�Q�[���N���A", strlen("�Q�[���N���A"), m_hFont) / 2;
		mojiy = APP_WNDSY / 2 - GetFontSizeToHandle(m_hFont)/2;
		DrawExtendStringToHandle(mojix, mojiy, 1, 1, "�Q�[���N���A", COLOR_WHITE, m_hFont, COLOR_GRAY);
		break;
	case GAME_OVER:
		mojix = APP_WNDSX / 2 - GetDrawStringWidthToHandle("�Q�[���I�[�o�[", strlen("�Q�[���I�[�o�["), m_hFont) / 2;
		mojiy = APP_WNDSY / 2 - GetFontSizeToHandle(m_hFont) / 2;
		DrawExtendStringToHandle(mojix, mojiy, 1, 1, "�Q�[���I�[�o�[", COLOR_WHITE, m_hFont, COLOR_GRAY);

		break;

	}
	
}

//	�V�[���I�����̌㏈��
void releaseGame1Scene(void) {


}

// ���蔻��R�[���o�b�N �@�@�@�����ł͗v�f���폜���Ȃ����ƁI�I
void  Game1SceneCollideCallback(int nSrc, int nTarget, int nCollideID) {

}


//�����蔻��
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


//�\���̔z��Ƃ̓����蔻��(���������ꍇ�Y���ԍ���Ԃ� ,������Ȃ�����-1)
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

	//�ǂꂩ�Ɠ��������瓖�������t���O��Ԃ�
	if (Ax1 < Bx2 && Bx1 < Ax2 && Ay1 < By2 && By1 < Ay2) {
		return 1;
	}
	
	//�ǂ�Ƃ�������Ȃ�������0��Ԃ�
	return -1;
	
}

//�g�p���Ă���\���̔z��ƁA�����蔻��̃^�[�Q�b�g���w�肵�A�������Ă��邩���肷��
//TODO:�����Ɨǂ��������肻�������ǎb��
int CharactorHitCheck(CharacterStatus* CharaA,int TargetID) {
	//������ID���擾
	int YourID = CharaA->charaID;

	//�\���̔z�������
	for (int CharaI = 0; CharaI < CHARA_NUM; CharaI++) {
		//����ł�����͖̂�������
		if (charactor[CharaI].deadFlag == CHARA_DEAD)continue;
		//�������g�������ꍇ����������(�����Ɠ����O���[�v�Ƃ̓����蔻����Ċ�{�Ȃ���ˁH
		if (charactor[CharaI].charaID == YourID)continue;
		//�����̎������������ꍇ����������
		if (charactor[CharaI].charaID == CharaA->propertyID)continue;

		//��������ID��������A�����蔻����s��
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

			//�ǂꂩ�Ɠ��������瓖�������t���O��Ԃ�
			if (Ax1 < Bx2 && Bx1 < Ax2 && Ay1 < By2 && By1 < Ay2) {
				//�R�C�c�������������̂ŁA���ʖ��߂�����
				CharaA->deadFlag = CHARA_DEAD;
				charactor[CharaI].deadFlag = CHARA_DEAD;
				return 1;
			}
		}
	}

	//�����I��������ǕԂ��Ă��Ȃ���������Ȃ������B
	return 0;

}