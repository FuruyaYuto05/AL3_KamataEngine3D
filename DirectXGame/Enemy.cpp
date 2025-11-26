#include "Enemy.h"
#include "MyMath.h"
#include "Player.h"
#include <cassert>
#include <numbers>
#include <cmath>

// デストラクタ追加
Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
}

// 02_09 スライド5枚目
void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLチェック
	assert(model);

	// 02_09 7枚目
	model_ = model;
	// 02_09 7枚目
	camera_ = camera;
	// 02_09 7枚目
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 02_09 7枚目 角度調整
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 02_09 16枚目
	velocity_ = {-kWalkSpeed, 0, 0};
	// 02_09 20枚目
	walkTimer = 0.0f;

	fireTimer_ = kFireInterval;
}

// 弾発射関数
void Enemy::Fire() {
	if (!player_ || !bulletModel_)
		return;

	// 敵の座標
	Vector3 enemyPos = GetWorldPosition();
	// プレイヤーの座標
	Vector3 playerPos = player_->GetWorldPosition();

	// 敵からプレイヤーへのベクトル
	Vector3 diff;
	diff.x = playerPos.x - enemyPos.x;
	diff.y = playerPos.y - enemyPos.y;
	diff.z = playerPos.z - enemyPos.z;


	// ベクトルの長さを計算
	float length = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	// 長さが0でない場合のみ正規化して発射
	if (length > 0.0f) {
		// 正規化（長さを1にする）
		Vector3 direction = diff;
		direction.x /= length;
		direction.y /= length;
		direction.z /= length;

		// 弾を生成
		EnemyBullet* newBullet = new EnemyBullet();
		Vector3 bulletVelocity;
		
		bulletVelocity.x = direction.x * kBulletSpeed;
		bulletVelocity.y = direction.y * kBulletSpeed;
		bulletVelocity.z = direction.z * kBulletSpeed;

		// 弾を初期化
		newBullet->Initialize(bulletModel_, enemyPos, bulletVelocity);

		// リストに追加
		bullets_.push_back(newBullet);
	}
}

// 02_09 スライド5枚目
void Enemy::Update() {

	// 02_09 16枚目 移動
	worldTransform_.translation_ += velocity_;

	// 02_09 20枚目
	walkTimer += 1.0f / 60.0f;

	// 02_09 23枚目 回転アニメーション
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

	// 02_09 スライド8枚目 ワールド行列更新
	WorldTransformUpdate(worldTransform_);


	// --- 発射ロジック ---
	fireTimer_--;
	if (fireTimer_ <= 0) {
		Fire();                     // 発射
		fireTimer_ = kFireInterval; // タイマーリセット
	}

	// --- 弾の更新 ---
	// 死亡フラグが立っている弾をリストから削除（メモリ解放も行う）
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 生きている弾を更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

// 02_09 スライド5枚目
void Enemy::Draw() {

	// 02_09 スライド9枚目  モデル描画
	model_->Draw(worldTransform_, *camera_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}

// 02_10 スライド14枚目
AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 02_10 スライド14枚目
Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

    // 02_10 スライド20枚目
void Enemy::OnCollision(const Player* player) {
	// --- プレイヤーの無敵状態チェック ---
	if (player->IsInvincible()) {
		return;
	}
	(void)player;
	
}
// --- プレイヤーの攻撃を受けた際の応答 ---
void Enemy::OnHitByPlayerAttack(const Player* player) {

	// ノックバックの強さ
	const float knockbackSpeedX = 0.1f;  // 水平方向の初速
	const float knockbackSpeedY = 0.05f; // 垂直方向の初速

	// プレイヤーの向きを取得し、ノックバック方向を決定
	if (player->GetDirection() == Player::LRDirection::kRight) {
		// プレイヤーが右を向いて攻撃 -> 敵は右にノックバック
		velocity_.x = knockbackSpeedX;
	} else {
		// プレイヤーが左を向いて攻撃 -> 敵は左にノックバック
		velocity_.x = -knockbackSpeedX;
	}

	velocity_.y = knockbackSpeedY;


}