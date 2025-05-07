#pragma once
#include "KamataEngine.h"



	// 引数は参照で渡しますが、戻り値は値渡しを使用
	KamataEngine::Matrix4x4 MakeAffineMatrix(
		KamataEngine::Vector3& scale,
		KamataEngine::Vector3& rotation,
		KamataEngine::Vector3& translation);


