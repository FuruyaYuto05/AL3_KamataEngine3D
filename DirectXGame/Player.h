#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include <numbers> 
using namespace KamataEngine;

class MapChipField;

// 02_10 21枚目
class Enemy;
class Bullet; 
class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角 02_07スライド16枚目
	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	/// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw();
	
	// getter(02_06スライド11枚目で追加)
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 02_06スライド28枚目で追加
	const Vector3& GetVelocity() const { return velocity_; }

	// 02_07 スライド4枚目
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// 02_10 10枚目 ワールド座標を取得
	Vector3 GetWorldPosition() const; 

	// 02_10 13枚目
	AABB GetAABB();

	// 02_10 21枚目 衝突応答
	void OnCollision(const Enemy* enemy);


	bool IsInvincible() const { return isInvincible_; }// 無敵中か
	bool IsAttacking() const { return isAttacking_; }// 攻撃中か

	//  プレイヤーのHPを取得
	int32_t GetHP() const { return hp_; }
	//  プレイヤーが生きているか（HPが0より大きいか）
	bool IsAlive() const { return hp_ > 0; }

	// 死亡フラグを取得するGetterを追加
	bool IsDead() const { return isDead_; }


	AABB GetAttackAABB() const;
	LRDirection GetDirection() const { return lrDirection_; }

	
	float attackTimer_ = 0.0f;                       
	static inline const float kAttackDuration = 0.1f; 

	// プレイヤーから弾のリストを取得
	std::list<Bullet*>& GetBullets() { return bullets_; }
	std::list<Bullet*> PopNewBullets();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Camera* camera_ = nullptr;
	// 02_05 移動量
	Vector3 velocity_ = {};
	// 02_05  フレームごとの加速度
	static inline const float kAcceleration = 0.1f;
	// 02_05 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;
	// 02_05 最高速度
	static inline const float kLimitRunSpeed = 0.3f;
	// 02_05 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;
	// 02_05 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 02_05 旋回タイマー
	float turnTimer_ = 0.0f;
	// 02_05 旋回時間 <秒>
	static inline const float kTimeTurn = 0.3f;
	// 02_05 着地フラグ
	bool onGround_ = true;
	// 02_05 ジャンプ定数もろもろ
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;

	// 02_07 マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// 02_07 キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// 02_07スライド34枚目
	static inline const float kBlank = 0.04f;
	
	// ローリング機能関連 
	bool isRolling_ = false;          // ローリング中か
	bool isInvincible_ = false;       // 無敵状態か
	float currentRollTime_ = 0.0f;    // ローリング開始からの経過時間
	Vector3 rollDirection_ = {};      // ローリングする方向
	float rollFirstRotationX_ = 0.0f; // ローリング開始時のX軸回転角度

	// ローリングのパラメータ
	static inline const float kRollSpeed = 0.2f;
	static inline const float kRollDuration = 0.3f;
	static inline const float kRollRotationX = std::numbers::pi_v<float> * 2.0f * 2.0f; // 2周分

	// 近接攻撃機能関連
	bool isAttacking_ = false;                        // 攻撃中か
	static inline const float kAttackCoolDown = 0.3f; // 攻撃アニメーションと硬直の総時間

	// チャージショット関連
	bool isCharging_ = false;                        // チャージ中か？
	float chargeTime_ = 0.0f;                        // チャージ時間
	static inline const float kChargeTimeMax = 2.0f; // 最大1.5秒チャージ
	bool prevCKey_ = false;                          // Cキー長押しの前フレーム状態
	//bool isShot_ = false;                            // 弾が発射されたか
	//float shotCoolDown = 0.0f;                       // 弾発射のクールダウン



	// 攻撃の当たり判定のサイズとオフセット
	static inline const float kAttackWidth = 1.2f;
	static inline const float kAttackHeight = 1.0f;
	static inline const Vector3 kAttackOffset = {0.5f, 0.0f, 0.0f}; // 自機中心から

	// 02_07スライド10枚目 移動入力
	void InputMove();

	// 攻撃・回避メソッド
	void StartRoll();                   // ローリングを開始する処理
	void HandleRoll(float deltaTime);   // ローリング中の移動と終了処理
	void StartAttack();                 // 攻撃を開始する処理
	void HandleAttack(float deltaTime); // 攻撃中の更新処理


	// 02_07 スライド12枚目
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};
	// 02_07 スライド13枚目
	void CheckMapCollision(CollisionMapInfo& info);
	// 02_07 スライド14枚目
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	// 02_07 スライド17枚目
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 02_08スライド14枚目 設置状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 02_08 スライド27枚目 壁接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	// 02_08スライド16枚目 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.0f;
	// 02_08スライド21枚目 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	// 02_08スライド27枚目 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;
	// ２段ジャンプできるかのフラグ
	bool can2Jump_ = false;

	// プレイヤーが生成した弾のリスト 
	std::list<Bullet*> bullets_;

	// HP関連  
	int32_t hp_ = 0;                              // 現在のHP
	static inline const int32_t kMaxHP = 5;      // 最大HP 
	static inline const int32_t kDamageValue = 1; // 敵の接触や弾で受けるダメージ

	bool isDead_ = false;


};