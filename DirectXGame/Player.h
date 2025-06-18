#pragma once
#include "KamataEngine.h"
#include <vector>

using namespace KamataEngine;
class MapChipField;
// 角
enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,

	kNumCorner
};

// マップの当たり判定情報
struct CollisionMapInfo {
	bool cailing = false;
	bool landing = false;
	bool hitWall = false;
	Vector3 move;
};


enum class LRDirection {
	kRight,
	kLeft,
};
// 自キャラ
class Player {
private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformPlayer_;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.5f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;

	

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 0.7f;
	static inline const float kJumpAcceleration = 1.0f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;



	

public:

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.0f;
	static inline const float kHeight = 0.0f;

	static inline const float kBlank = 1;
	

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 移動入力
	void InputMove();
	// 旋回制御
	void AnimateTurn();

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 描画
	void Draw();

	void CheckMapCollision(CollisionMapInfo& info) ;

	void CheckMapCollisionUp(CollisionMapInfo& info);
	
	
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void CheckMove(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
};
