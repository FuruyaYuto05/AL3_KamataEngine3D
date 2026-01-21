#define NOMINMAX

#include "Player.h"
#include "MapChipField.h"
#include "MyMath.h"
#include "Bullet.h"
#include "Enemy.h"

#include <algorithm>
#include <cassert>
#include <numbers>

// --- Player::Initialize ---
void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	// モデル
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;

	// ローリング関連の初期化 
	isRolling_ = false;
	isInvincible_ = false;
	currentRollTime_ = 0.0f;

	// 攻撃関連の初期化
	isAttacking_ = false;
	attackTimer_ = 0.0f;

	// HPの初期化
	hp_ = kMaxHP;
}

// --- Player::StartRoll  ---
void Player::StartRoll() {
	isRolling_ = true;
	isInvincible_ = true; 
	currentRollTime_ = 0.0f;

	// ローリング前のX軸回転角度を記録
	rollFirstRotationX_ = worldTransform_.rotation_.x;

	// ローリングaの方向を決定（現在の向きを使用）
	if (lrDirection_ == LRDirection::kRight) {
		rollDirection_ = {1.0f, 0.0f, 0.0f};  // 右方向
	} else {                                  // kLeft
		rollDirection_ = {-1.0f, 0.0f, 0.0f}; // 左方向
	}

	// Y, Z方向は動かさない
	rollDirection_.y = 0.0f;
	rollDirection_.z = 0.0f;

	// ローリング開始速度をリセット
	velocity_ = {0.0f, 0.0f, 0.0f};
}

// --- Player::HandleRoll ---
void Player::HandleRoll(float deltaTime) {

	// 1. 移動量の計算
	Vector3 move = rollDirection_ * kRollSpeed;

	// 2. 当たり判定用の構造体を作成
	CollisionMapInfo collisionInfo;
	collisionInfo.move = move; // 計算した移動量をセット
	collisionInfo.landing = false;
	collisionInfo.hitWall = false;

	// 3. マップ衝突判定を行う (これで移動量 move が壁に合わせて補正されます)
	CheckMapCollision(collisionInfo);

	// 4. 補正された移動量を座標に加算
	worldTransform_.translation_ += collisionInfo.move;

	// 5. (オプション) 壁にぶつかったらローリングを強制終了させたい場合
	// if (collisionInfo.hitWall) {
	//    // 必要であればここで終了処理など
	// }

	// --- 以下、時間更新などの処理はそのまま ---

	// 時間の更新
	currentRollTime_ += deltaTime;

	// ローリング回転処理
	float t = currentRollTime_ / kRollDuration;
	t = std::min(t, 1.0f);
	float destinationRotationX = rollFirstRotationX_ + kRollRotationX;
	worldTransform_.rotation_.x = EaseInOut(rollFirstRotationX_, destinationRotationX, t);

	// ローリング終了のチェック
	if (currentRollTime_ >= kRollDuration) {
		// ローリングを終了する
		isRolling_ = false;
		isInvincible_ = false;
		currentRollTime_ = 0.0f;

		// 終了時の速度とX軸回転をリセット
		velocity_ = {0.0f, 0.0f, 0.0f};
		worldTransform_.rotation_.x = 0.0f;
	}
}

// --- Player::StartAttack ---
void Player::StartAttack() {
	isAttacking_ = true;
	attackTimer_ = 0.0f;
	velocity_ = {0.0f, 0.0f, 0.0f}; 

}

// --- Player::HandleAttack ---
void Player::HandleAttack(float deltaTime) {
	attackTimer_ += deltaTime;

	
	if (attackTimer_ >= kAttackCoolDown) {
		isAttacking_ = false;
	}	
}

// --- Player::PopNewBullets ---
// GameSceneが新しい弾を取得したら、このリストを空にする
std::list<Bullet*> Player::PopNewBullets() {
	std::list<Bullet*> result = std::move(bullets_);
	bullets_.clear();
	return result;
}

// 登場演出用関数Player
void Player::UpdateIntro() {
	// くるくる回る
	worldTransform_.rotation_.y += 0.3f;

	// 行列の更新だけ行う
	WorldTransformUpdate(worldTransform_);
}

void Player::OnIntroFinished() {
	// 向きを「右（初期状態）」に戻す
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// 念のため速度もリセット
	velocity_ = {0.0f, 0.0f, 0.0f};

	// 行列を更新して確定させる
	WorldTransformUpdate(worldTransform_);
}

// --- Player::InputMove ---
void Player::InputMove() {

	// 左右のスティックの状態を取得
	XINPUT_STATE state;
	Input::GetInstance()->GetJoystickState(0, state);

	// 左スティックのX軸の値を取得
	//short thumbX = state.Gamepad.sThumbLX;

	// デッドゾーンを設定
	//const short DEADZONE = 16000;

	// 優先度制御用のフラグ
	bool keyboardInputX = false;

	if (onGround_) {

		// 地上での左右移動操作

		// キーボードの左右移動操作をまずチェック
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			keyboardInputX = true; // キーボード入力あり

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					// 旋回の最初は移動減衰をかける
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration / 60.0f;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					// 旋回の最初は移動減衰をかける
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration / 60.0f;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;
		}


		// 速度の制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		// ほぼ0の場合に0にする
		if (std::abs(velocity_.x) <= 0.0001f) {
			velocity_.x = 0.0f;
		}

		// 一段目ジャンプ操作（DIK_UP または Aボタン）
		if (Input::GetInstance()->TriggerKey(DIK_UP) ) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration / 60.0f, 0);
		}

	} else {
		// 空中での処理（落下速度と空中移動）

		//if (can2Jump_) {
		//	if (Input::GetInstance()->TriggerKey(DIK_UP) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		//		// 一段目のジャンプ速度をリセット
		//		velocity_.y = 0.0f;
		//		// ジャンプ初速
		//		velocity_ += Vector3(0, kJumpAcceleration / 60.0f, 0);
		//		can2Jump_ = false;
		//	}
		//}
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration / 60.0f, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		// 空中移動のキーボード優先ロジック
		keyboardInputX = false;

		// キーボード入力チェック
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			keyboardInputX = true;
			// キーボードでの空中加速
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				velocity_.x += kAcceleration / 60.0f;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				velocity_.x -= kAcceleration / 60.0f;
			}
		}


		// 空中での速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}
}

// --- Player::CheckMapCollision (衝突判定のラッパー) ---
// 02_07 スライド13枚目
void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

// --- Player::CheckMapCollisionUp (上方向の衝突判定) ---
// 02_07 スライド14枚目(上下左右全て)
void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	// 02_07スライド20枚目 上昇あり?
	if (info.move.y <= 0) {
		return;
	}

	// 02_07 スライド19枚目（下の文も）
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 02_07 スライド28枚目（下の文も）
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？ 02_07 スライド34枚目
	if (hit) {
		// 現在座標が壁の外か判定
		IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.ceiling = true;
		}
	}
}

// --- Player::CheckMapCollisionDown (下方向の衝突判定) ---
void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	// 02_08 スライド7枚目 下降あり？
	if (info.move.y >= 0) {
		return;
	}

	// 02_08 スライド7枚目（下の文も）
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

// --- Player::CheckMapCollisionRight (右方向の衝突判定) ---
void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

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

// --- Player::CheckMapCollisionLeft (左方向の衝突判定) ---
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

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

// --- Player::UpdateOnGround (接地状態の切り替え) ---
// 02_08スライド14枚目 設置状態の切り替え処理
void Player::UpdateOnGround(const CollisionMapInfo& info) {

	if (onGround_) {
		// 02_08スライド18枚目 ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			// 落下判定
			// 落下なら空中状態に切り替え

			// 02_08スライド19枚目(このブロック全部)
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			bool hit = false;

			MapChipType mapChipType;

			// 左下点の判定
			IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				//				DebugText::GetInstance()->ConsolePrintf("jump");
				onGround_ = false;
			}
		}
	} else {
		// 02_08スライド16枚目 地面に接触している場合の処理
		if (info.landing) {
			// 着地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロに
			velocity_.y = 0.0f;

			can2Jump_ = true;
		}
	}
}

// --- Player::UpdateOnWall ---
// 02_08スライド27枚目 壁接地中の処理
void Player::UpdateOnWall(const CollisionMapInfo& info) {

	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

// --- Player::CornerPosition ---
// 02_07 スライド17枚目
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //    kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //    kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //    kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //    kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::Update() {

	const float deltaTime = 1.0f / 60.0f;

	if (!IsAlive()) {
		WorldTransformUpdate(worldTransform_);
		return;
	}

	Input* input = Input::GetInstance();

	// 現在のCキー状態
	bool cNow = input->PushKey(DIK_C);

	// 1. ローリング中の処理
	if (isRolling_) {
		HandleRoll(deltaTime);
	}
	// 2. 攻撃中の処理
	else if (isAttacking_) {
		HandleAttack(deltaTime);
	}
	// 3. 通常時
	else {

		// ────────────────────────────────────
		//   チャージショット処理 
		// ────────────────────────────────────

		// チャージ開始（押した瞬間）
		if (!isCharging_ && input->TriggerKey(DIK_C)) {
			isCharging_ = true;
			chargeTime_ = 0.0f;
		}

		// チャージ中（押している間）
		if (isCharging_ && cNow) {
			chargeTime_ += deltaTime;
			if (chargeTime_ > kChargeTimeMax) {
				chargeTime_ = kChargeTimeMax;
			}
		}
		
		// 単発かチャージか
		// キー離し判定（前フレーム押していた & 今フレーム離した）
		if (isCharging_ && prevCKey_ && !cNow) {

			isCharging_ = false;

			Bullet* newBullet = new Bullet();
			newBullet->Initialize(nullptr, worldTransform_.translation_, lrDirection_);

			// 0.25秒未満 → 通常ショット
			if (chargeTime_ < 0.25f) {

				
				bullets_.push_back(newBullet);
			}
			// 0.25秒以上 → チャージショット
			else {
				float t = chargeTime_ / kChargeTimeMax;

				// サイズを大きくする
				float scale = 0.2f + t * 0.8f;
				newBullet->SetScale({scale, scale, scale});

				// 速度強化
				float speed = 0.3f + t * 0.6f;
				newBullet->SetSpeed(speed);

				bullets_.push_back(newBullet);
			}
		}

		// ────────────────────────────────────
		//   攻撃 / ローリング / 移動 
		// ────────────────────────────────────

		// 攻撃
		if (input->TriggerKey(DIK_Z)) {
			StartAttack();
		}
		// ローリング
		else if (input->TriggerKey(DIK_SPACE)) {
			StartRoll();
		}
		// 通常移動
		else {

			InputMove();

			CollisionMapInfo collisionMapInfo = {};
			collisionMapInfo.move = velocity_;
			collisionMapInfo.landing = false;
			collisionMapInfo.hitWall = false;

			// マップ衝突
			CheckMapCollision(collisionMapInfo);

			// 移動適用
			worldTransform_.translation_ += collisionMapInfo.move;

			if (collisionMapInfo.ceiling) {
				velocity_.y = 0;
			}

			UpdateOnWall(collisionMapInfo);

			UpdateOnGround(collisionMapInfo);

			// 旋回処理
			if (turnTimer_ > 0.0f) {
				turnTimer_ = std::max(turnTimer_ - deltaTime, 0.0f);

				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

				worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
			}
		}
	}


	// 前フレームのCキー状態を更新
	prevCKey_ = cNow;

	// 行列更新
	WorldTransformUpdate(worldTransform_);
}


// --- Player::Draw ---
void Player::Draw() {

	if (IsAlive()) {

		// モデル描画
		model_->Draw(worldTransform_, *camera_);
	}
}

// --- Player::GetWorldPosition ---
// 02_10 10枚目
Vector3 Player::GetWorldPosition() const { // ← GetAttackAABBから呼ばれるため const を追加

	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

// --- Player::GetAABB ---
// 02_10 14枚目
AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// --- Player::GetAttackAABB ---
AABB Player::GetAttackAABB() const {

	// プレイヤーのワールド座標
	Vector3 playerWorldPos = GetWorldPosition(); 

	// 攻撃判定のオフセットを初期設定
	Vector3 adjustedOffset = kAttackOffset;

	// プレイヤーの向きが左の場合、オフセットを反転して攻撃方向を合わせる
	if (lrDirection_ == LRDirection::kLeft) {
		adjustedOffset.x *= -1.0f;
	}

	// 攻撃の中心座標
	Vector3 attackCenter = playerWorldPos + adjustedOffset;

	AABB aabb;
	
	aabb.min = {attackCenter.x - kAttackWidth / 2.0f, attackCenter.y - kAttackHeight / 2.0f, attackCenter.z - kAttackWidth / 2.0f};
	aabb.max = {attackCenter.x + kAttackWidth / 2.0f, attackCenter.y + kAttackHeight / 2.0f, attackCenter.z + kAttackWidth / 2.0f};

	return aabb;
}

// --- Player::OnCollision ---
void Player::OnCollision(const Enemy* enemy) {

	// 無敵状態なら、衝突応答をスキップ
	if (isInvincible_) {
		return;
	}

	// 1. HPを減らす
	hp_ -= kDamageValue;
	hp_ = std::max(hp_, 0);

	if (hp_ <= 0) {
		isDead_ = true;
	}

	// ノックバック方向の計算
	// 敵の位置と自分の位置を取得

	Vector3 enemyPos = enemy->GetWorldPosition();
	Vector3 playerPos = GetWorldPosition();

	// 敵から自分へのベクトル（＝吹っ飛ぶ方向）
	Vector3 knockbackDir = playerPos - enemyPos;
	knockbackDir.y = 0.0f;                  // 上下方向の要素は一旦消す（真横に計算するため）
	knockbackDir = Normalize(knockbackDir); // ベクトルを正規化（長さ1にする）

	// もし位置が完全に重なっていて計算できない場合は、とりあえず右へ
	if (Length(knockbackDir) == 0.0f) {
		knockbackDir = {1.0f, 0.0f, 0.0f};
	}

	// ノックバック速度を与える
	// 横方向の強さ
	float knockbackPowerHorizontal = 0.3f; 
	// 上方向の強さ（少し浮かせるとそれっぽい）
	float knockbackPowerVertical = 0.2f;

	velocity_.x = knockbackDir.x * knockbackPowerHorizontal;
	velocity_.y = knockbackPowerVertical;


}