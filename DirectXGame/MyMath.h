#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

Matrix4x4 MakeAffineMatrix(Vector3& scale, Vector3& rotation, Vector3& translation);

// イージング
float EaseInOut(float x1, float x2, float t);