#include "GameScene.h"
#include "MyMath.h"
#include "EnemyBullet.h"

using namespace KamataEngine;

GameScene::~GameScene() {

	delete sprite_;
	delete model_;

	delete block_model_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete deathParticles_;
	delete deathParticle_model_;

	// 02_09 10枚目 敵クラス削除→02_10 6枚目で削除
	//	delete enemies_;
	
	// 02_10 6枚目 敵クラス削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// 敵の弾モデルの削除
	delete enemy_bullet_model_;

	// 弾の削除　　
	for (Bullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();

}

bool IsAABBCollide(const AABB& a, const AABB& b) { return a.min.x <= b.max.x && a.max.x >= b.min.x && a.min.y <= b.max.y && a.max.y >= b.min.y && a.min.z <= b.max.z && a.max.z >= b.min.z; }

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, {70, 50});
	// 3Dモデル生成
	model_ = Model::Create();
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	// カメラ初期化
	camera_.Initialize();

	// ブロックモデル
	block_model_ = Model::CreateFromOBJ("block");

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	attackAABB_model_ = block_model_; // 一時的に

	// 当たり判定用ワールドトランスフォームの初期化
	attackAABB_worldTransform_.Initialize();

	// 02_03 skydome生成
	skydome_ = new Skydome();
	// 初期化
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelSkydome_, &camera_);

	// 02_04マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// 02_07 マップチップクラスを作ってからプレイヤークラスを作る
	// という順番に入れ替える
	// 02_01から追加 プレイヤー生成
	player_ = new Player();

	// プレイヤーモデル
	player_model_ = Model::CreateFromOBJ("player");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// 02_07 スライド5枚目
	player_->SetMapChipField(mapChipField_);

	player_->Initialize(player_model_, &camera_, playerPosition);

	// 02_06カメラコントローラ スライド13枚目
	CameraController_ = new CameraController(); // 生成
	CameraController_->Initialize(&camera_);    // 初期化
	CameraController_->SetTarget(player_);      // 追従対象セット
	CameraController_->Reset();                 // リセット

	// 02_06カメラコントローラ スライド18枚目
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CameraController_->SetMovableArea(cameraArea);

	// 02_09 10枚目 敵クラス → 02_10の5枚目で削除
	//	enemy_ = new Enemy();
	// 02_09 10枚目 敵モデル
	enemy_model_ = Model::CreateFromOBJ("enemy");
	// 02_09 10枚目 敵位置決めて敵クラス初期化 → 02_10の5枚目で削除
	//	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	// enemy_->Initialize(enemy_model_, &camera_, enemyPosition);
	
	// 敵の弾用モデルの生成 (objファイル名は "bullet" としていますが、別のファイルがあれば書き換えてください)
	enemy_bullet_model_ = Model::CreateFromOBJ("bullet");

	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");


	// 02_10 5枚目（for文の中身全部）
	for (int32_t i = 0; i < 1; ++i) {
		Enemy* newEnemy = new Enemy();

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + i * 2, 18);

		newEnemy->Initialize(enemy_model_, &camera_, enemyPosition);

		// 敵にプレイヤーの情報と、今回作った弾モデルを渡す
		newEnemy->SetPlayer(player_);
		newEnemy->SetBulletModel(enemy_bullet_model_);

		newEnemy->SetMapChipField(mapChipField_);

		newEnemy->SetDeathParticleModel(deathParticle_model_);


		enemies_.push_back(newEnemy);
	}
	

	// 02_11 16枚目 敵モデル
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");

	// 02_11 16枚目
	//deathParticles_ = new DeathParticles;
	//deathParticles_->Initialize(deathParticle_model_, &camera_, playerPosition);
	
	//弾モデル
	bullet_model_ = Model::CreateFromOBJ("bullet");
	phase_ = Phase::kPlay;
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}


void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		if (!player_) {
			return;
		}
		// 02_12 13枚目 if文から中身まで全部実装
		// Initialize関数のいきなりパーティクル発生処理は消す
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		break;
	}
}


// ゲームシーン更新
void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		isPaused_ = !isPaused_; // trueとfalseを反転
	}

	// ポーズ中なら、更新処理を飛ばして終了（描画は続くので画面は止まって見える）
	if (isPaused_) {
		return;
	}

	// ===== 敵HPが0ならゲームクリア（即遷移）=====
	for (Enemy* enemy : enemies_) {
		if (enemy && enemy->GetHP() <= 0) {
			endStatus_ = EndStatus::GameClear;
			finished_ = true;
			return;
		}
	}
	// ============================================


	ChangePhase();

	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		break;
	case Phase::kDeath:
		// 02_12 34枚目 デス演出フェーズの処理
		// deathParticles_->IsFinished関数をDeathParticles.hに実装
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	player_->Update();
	skydome_->Update();
	CameraController_->Update();
	

	// 02_09 12枚目 敵更新 → 02_10 7枚目で更新
	//	enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// 弾の生成と更新
	// 1. プレイヤーから生成された弾を取得し、全体のリストに追加
	std::list<Bullet*> newBullets = player_->PopNewBullets();
	for (Bullet* newBullet : newBullets) {
		// Bulletのモデルを設定
		newBullet->SetModel(bullet_model_);
		bullets_.push_back(newBullet);
	}

	// ─────────────────────────────
	// プレイヤーの弾 vs 敵
	// ─────────────────────────────
	for (Bullet* bullet : bullets_) {

		if (!bullet || bullet->IsDead()) {
			continue;
		}

		AABB bulletAABB = bullet->GetAABB();

		for (Enemy* enemy : enemies_) {

			if (!enemy)
				continue;

			AABB enemyAABB = enemy->GetAABB();

			if (IsAABBCollide(bulletAABB, enemyAABB)) {

				enemy->OnHitByPlayerAttack(player_);
				bullet->SetDead(true);

				break;
			}
		}
	}



	// 2. 弾の更新
	for (Bullet* bullet : bullets_) {
		bullet->Update();
	}

	// 3. 寿命が尽きた弾を削除
	bullets_.remove_if([](Bullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

#ifdef _DEBUG
	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//	// フラグをトグル
	//	isDebugCameraActive_ = !isDebugCameraActive_;
	//}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			// アフィン変換～DirectXに転送
			WorldTransformUpdate(*worldTransformBlock);
		}
	}

	// デバッグカメラの更新
	debugCamera_->Update();

	// 攻撃中かつ、当たり判定が有効な期間（kAttackDuration）の場合
	if (player_->IsAttacking() && player_->attackTimer_ < player_->kAttackDuration) {

		isAttackAABBDrawn_ = true;

		// 攻撃のAABBを取得
		AABB attackAABB = player_->GetAttackAABB();

		// 当たり判定ボックスの中心を計算
		Vector3 center = (attackAABB.min + attackAABB.max) * 0.5f;

		// 当たり判定ボックスのサイズを計算 (max - min)
		Vector3 size = attackAABB.max;
		size -= attackAABB.min;

		// WorldTransformに適用
		attackAABB_worldTransform_.translation_ = center;
		attackAABB_worldTransform_.scale_ = size;

	} else {
		isAttackAABBDrawn_ = false;
	}

	// 当たり判定用トランスフォームの行列を更新
	WorldTransformUpdate(attackAABB_worldTransform_);

	// 02_10 22枚目 衝突判定
	CheckAllCollisions();

	

	// 02_11 18枚目 デスパーティクルあれば更新
	if (deathParticles_) {
		deathParticles_->Update();
	}

	// ─────────────────────────────
	// HP0 の敵を削除
	// ─────────────────────────────
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsReadyToRemove()) {
			delete enemy;
			return true;
		}
		return false;
	});

	  // ① 自機HPが0ならゲームオーバー
	if (player_ && player_->GetHP() <= 0) {
		endStatus_ = EndStatus::GameOver;
		return;
	}
	
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		finished_ = true;
		
		endStatus_ = EndStatus::Reset;
		return; // リセットが押されたら、このフレームの他の処理は中断
	}

}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 自キャラの描画
	player_->Draw();

	// 天球描画
	skydome_->Draw();


		// 02_11 18枚目
		if (deathParticles_) {
			deathParticles_->Draw();
		}
	
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			block_model_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 02_09 12枚目 敵更新 → 02_10 7枚目で更新
	//	enemy_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 弾の描画
	for (Bullet* bullet : bullets_) {
		bullet->Draw(camera_);
	}

	// --- 攻撃判定のデバッグ描画 ---
	if (isAttackAABBDrawn_) {
		// 当たり判定のモデルを描画
		attackAABB_model_->Draw(attackAABB_worldTransform_, camera_);
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
}

// 02_10 16枚目
void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {

			if (enemy->IsDying() || enemy->IsReadyToRemove()) {
				continue;
			}

			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
			// 自キャラと「この敵が撃った弾」の当たり判定
			const auto& enemyBullets = enemy->GetBullets(); // 敵から弾リストをもらう
			for (EnemyBullet* bullet : enemyBullets) {
				// 弾のAABBを取得
				AABB bulletAABB = bullet->GetAABB();

				if (IsCollision(aabb1, bulletAABB)) {
					// プレイヤーにダメージ (引数は撃った敵)
					player_->OnCollision(enemy);
					// 弾を消す
					bullet->OnCollision();
				}
			}
		}
	}
#pragma endregion

	// --- 自キャラの攻撃判定と敵キャラの当たり判定 ---
#pragma region 自キャラの攻撃判定 vs 敵
	if (player_->IsAttacking() && player_->attackTimer_ < player_->kAttackDuration) {

		// 自キャラの攻撃の座標
		aabb1 = player_->GetAttackAABB();

		for (Enemy* enemy : enemies_) {
			// 敵の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
			
				enemy->OnHitByPlayerAttack(player_);
			}
		}
	}
#pragma endregion
}
