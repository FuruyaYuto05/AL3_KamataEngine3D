#include "MyMath.h"
using namespace KamataEngine;
using namespace MathUtility;


KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation) {
	
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	Matrix4x4 matRotX = MakeRotateXMatrix(rotation.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	Matrix4x4 matTrans = MakeTranslateMatrix(translation);

	Matrix4x4 matWorld = matScale * matRot * matTrans;
	
	return matWorld; 
}
