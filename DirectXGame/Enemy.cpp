#define NOMINMAX

#include "Enemy.h"
#include "MyMath.h"
#include "Player.h"
#include <cassert>
#include <numbers>
#include <cmath>
#include "MapChipField.h"

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

//void Enemy::UpdateOnGround(const CollisionMapInfo& info) {
//
//	if (onGround_) {
//		// 02_08スライド18枚目 ジャンプ開始
//		if (velocity_.y > 0.0f) {
//			onGround_ = false;
//		} else {
//			// 落下判定
//			// 落下なら空中状態に切り替え
//
//			// 02_08スライド19枚目(このelseブロック全部)
//			std::array<Vector3, kNumCorner> positionsNew;
//
//			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
//				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
//			}
//
//			bool hit = false;
//
//			MapChipType mapChipType;
//
//			// 左下点の判定
//			IndexSet indexSet;
//			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
//			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//			if (mapChipType == MapChipType::kBlock) {
//				hit = true;
//			}
//
//			// 右下点の判定
//			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
//			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//			if (mapChipType == MapChipType::kBlock) {
//				hit = true;
//			}
//
//			// 落下開始
//			if (!hit) {
//				//				DebugText::GetInstance()->ConsolePrintf("jump");
//				onGround_ = false;
//			}
//		}
//	} else {
//		// 02_08スライド16枚目 地面に接触している場合の処理
//		if (info.landing) {
//			// 着地状態に切り替える（落下を止める）
//			onGround_ = true;
//			// 着地時にX速度を減衰
//			velocity_.x *= (1.0f - kAttenuationLanding);
//			// Y速度をゼロに
//			velocity_.y = 0.0f;
//
//			can2Jump_ = true;
//		}
//	}
//}
//
//// 壁接地中の処理
//void Enemy::UpdateOnWall(const CollisionMapInfo& info) {
//
//	if (info.hitWall) {
//		velocity_.x *= (1.0f - kAttenuationWall);
//	}
//}

// 02_09 スライド5枚目
void Enemy::Update() {

	walkTimer += 1.0f / 60.0f;

	// 回転アニメーション
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

	// ワールド行列更新
	WorldTransformUpdate(worldTransform_);

	/*CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;*/

	// 移動
	worldTransform_.translation_ += velocity_;
	//worldTransform_.translation_ += collisionMapInfo.move;

	// マップ衝突
	//CheckMapCollision(collisionMapInfo);

	/*UpdateOnWall(collisionMapInfo);

	UpdateOnGround(collisionMapInfo);*/

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

void Enemy::CheckMapCollision(CollisionMapInfo& info) {

	//CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

// ---Enemy::CheckMapCollisionDown(下方向の衝突判定)---
void Enemy::CheckMapCollisionDown(CollisionMapInfo& info) {

	// 02_08 スライド7枚目 下降あり？
	if (info.move.y >= 0) {
		return;
	}

	// 02_08 スライド7枚目（下のfor文も）
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 02_08 スライド8枚目(右下、左下の判定まで)
	MapChipType mapChipType;

	// フラグ初期化
	bool hit = false;

	// 左下の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 02_08スライド11枚目 ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

// --- CheckMapCollisionRight (右方向の衝突判定) ---
void Enemy::CheckMapCollisionRight(CollisionMapInfo& info) {

	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右側の当たり判定
	bool hit = false;

	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// 現在座標が壁の外か判定
		IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}

// --- CheckMapCollisionLeft (左方向の衝突判定) ---
void Enemy::CheckMapCollisionLeft(CollisionMapInfo& info) {

	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右側の当たり判定
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// 現在座標が壁の外か判定
		IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		}
	}
}



Vector3 Enemy::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //    kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //    kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //    kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //    kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}