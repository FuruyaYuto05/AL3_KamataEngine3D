#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include <array>
#include <numbers>

using namespace KamataEngine;

class DeathParticles {
public:
	/// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw();

private:
	// モデルのポインタ
	Model* model_ = nullptr;

	// カメラのポインタ
	Camera* camera_ = nullptr;

	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;

	//存続期間
	static inline const float kDuration = 1.5f;

	//移動の速さ
	static inline const float kSpeed = 0.5f;

	//分割した１個分の角度
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2.0f / kNumParticles;

	//終了フラグ
	bool isFinished_ = false;

	//終了時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;

	//色の数値
	Vector4 color_;

};
