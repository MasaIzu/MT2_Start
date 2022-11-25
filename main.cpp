#include <DxLib.h>
#include "Vector3.h"
#include <cstring> //memcpy
#include<vector>

//�����̕`��
//Dxlib => int DrawLine3D(VECTOR Pos1,VECTOR Pos2,const unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

//�J�����̈ʒu�Ǝp���̐ݒ�
//Dxlib => int SetCameraPositionAndTargetAndUpVec(VECTOR Position, VECTOR Target, VECTOR Up);
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,	//�J�����̈ʒu
	const Vector3& cameraTarget,	//�J�����̒����_
	const Vector3& cameraUp			//�J�����̏�̌���
);

//���̕`��
//Dxlib => int DrawSphere3D(VECTOR CenterPos,float r,int DivNum,unsigned int DifColor,unsigned int SpcColor,int FillFlag);
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

//�֐��v���g�^�C�v�錾
void DrawAxis3D(const float length);	//x,y,z���̕`��


//����X�̏W��(vector�R���e�i),�⊮�����Ԃ̓Y���A���Ԍo�ߗ�
Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	const int WindowWidth = 1024;
	const int WindowHeight = 576;

	ChangeWindowMode(true);//�E�B���h�E���[�h�ɂȂ�
	SetGraphMode(WindowWidth, WindowHeight, 32);//��ʃ��[�h�̃Z�b�g
	SetBackgroundColor(0, 0, 64);//�w�i�FRGB
	if (DxLib_Init() == -1) { return -1; }// DXlib�̏�����
	SetDrawScreen(DX_SCREEN_BACK);// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);
	// �y�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(TRUE);

	////�J�����̏�����
	//Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
	//Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	//Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//�N���b�v��        �߁@�@�@ ��
	SetCameraNearFar(1.0f, 1000.0f);//�J�����̗L���͈͂̐ݒ�
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);//��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(
		Vector3(-20.0f, 20.0f, -200.0f),			//�J�����̈ʒu
		//Vector3(0.0f, 200.0f, 0.0f),				//�J�����̈ʒu
		Vector3(0.0f, 0.0f, 0.0f),				//�J�����̒����_
		Vector3(0.0f, 0.0f, 1.0f)				//�J�����̏�̌���
	);

	//���Ԍv�Z�ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//��ԂŎg���f�[�^
	//start �� end ��5�b�Ŋ���������
	Vector3 p0(-100.0f, 0, 0);			//�X�^�[�g�n�_
	Vector3 p1(-50.0f, 50.0f, 50.0f);	//����_����1
	Vector3 p2(50.0f, -30.0f, -50.0f);	//����_����2
	Vector3 p3(100.0f, 0.0f, 0.0f);		//�S�[���n�_

	std::vector<Vector3>points{ p0,p0,p1,p2,p3,p3 };

	float maxTime = 5.0f;				//�S�̎���[s]
	float timeRate;						//�������Ԃ��i�񂾂�

	//���̈ʒu
	Vector3 position;

	//���s�O�ɃJ�E���g�l���擾
	startCount = GetNowHiPerformanceCount(); //long long int�^ 64bit int

	size_t startIndex = 1;

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		//// �X�V����

		//[R]�Ń��Z�b�g
		if (CheckHitKey(KEY_INPUT_R)) {
			startCount = GetNowHiPerformanceCount();
			startIndex = 1;
		}

		//�o�ߎ���(elapsedTime[s])�̌v�Z
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

		//�X�^�[�g�n�_		: start
		//�G���h�n�_		: end
		//�o�ߎ���		: elapsed[s]
		//�ړ������̗�	(�o�ߎ���/�S�̎���) : timeRate(��)

		timeRate = elapsedTime / maxTime;
		//timeRate = min(elapsedTime / maxTime, 1.0f);

		if (timeRate >= 1.0f) {
			if (startIndex < points.size() - 3) {

				startIndex += 1;
				timeRate -= 1.0f;
				startCount = GetNowHiPerformanceCount();
			}
			else {
				timeRate = 1.0f;
			}
		}
		position = splinePosition(points, startIndex, timeRate);


		//Vector3 a = lerp(p0, p1, timeRate);
		//Vector3 b = lerp(p1, p2, timeRate);
		//Vector3 c = lerp(p2, p3, timeRate);

		//Vector3 d = lerp(a, b, timeRate);
		//Vector3 e = lerp(b, c, timeRate);

		//position = lerp(d, e, timeRate);
		//position = easeIn(start,end,timerate);
		//position = easeOut(start,end,timerate);
		//position = easeInOut(start,end,timerate);

		// �`�揈��
		ClearDrawScreen();	//��ʂ�����
		DrawAxis3D(500.0f);	//xyz���̕`��

		//���̕`��
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p0, 2.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p1, 2.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p2, 2.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p3, 2.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f[s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R]: Restart");
		//�t���b�v����
		ScreenFlip();
	}

	//20�~���b�ҋ@(�^��60FPS)
	WaitTimer(20);

	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

//x,y,z���̕`��
void DrawAxis3D(const float length) {
	//���̐��̕`��
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}

//�L�[����̕`��
void DrawKeyOperation() {
	const unsigned white = GetColor(255, 255, 255);

	DrawFormatString(10, 20 * 1, white, "[W][E][R]  R : ���Z�b�g");
	DrawFormatString(10, 20 * 2, white, "[A][S][D] AD : y���܂��̉�]");
	DrawFormatString(10, 20 * 3, white, "[Z]	   WS : X���܂��̉�]");
	DrawFormatString(10, 20 * 4, white, "		�@�@EZ : Z���܂��̉�]");
}

//�ȍ~�ADxlib�̊e�֐���Vector3�^ Matrix4�^ �𗘗p�ł���悤�ɂ���֐��Q
//���̕`��
//Dxlib => int DrawSphere3D(VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };

	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

//�����̕`��
//Dxlib => int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color) {
	VECTOR p1 = { Pos1.x, Pos1.y, Pos1.z };
	VECTOR p2 = { Pos2.x, Pos2.y, Pos2.z };

	return DrawLine3D(p1, p2, Color);
}

// �J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,  // �J�����̈ʒu
	const Vector3& cameraTarget,    // �J�����̒����_
	const Vector3& cameraUp         // �J�����̏�̌���
) {
	VECTOR position = { cameraPosition.x,cameraPosition.y,cameraPosition.z };
	VECTOR target = { cameraTarget.x,cameraTarget.y,cameraTarget.z };
	VECTOR up = { cameraUp.x,cameraUp.y,cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t) {

	//��Ԃ��ׂ��_�̐�
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}