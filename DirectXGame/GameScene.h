#pragma once
#include "CameraController.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "skydome.h"
#include <vector>
#include "DeathParticles.h"
#include "Bullet.h"

using namespace KamataEngine;

// ゲームシーンクラス
class GameScene {
public:
	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	void GenerateBlocks();

	// 02_10 16枚目 衝突判定と応答
	void CheckAllCollisions();

	bool IsFinished() const { return finished_; }
	
	

		// 終了ステータスを外部へ
	enum class EndStatus { None, GameOver, GameClear, Reset };
	EndStatus GetEndStatus() const { return endStatus_; }

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// カメラ
	Camera camera_;
	// サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;

	// ここから02_01からの追加
	//  プレイヤー
	Player* player_ = nullptr;
	// プレイヤーモデル
	Model* player_model_ = nullptr;

	// ブロックモデル
	Model* block_model_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 02_03 天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// 02_04 マップチップフィールド
	MapChipField* mapChipField_;

	// 02_06カメラ移動
	CameraController* CameraController_ = nullptr;

	// 02_09 10枚目 エネミークラス
	// Enemy* enemy_ = nullptr;//02_10で削除
	// 02_09 10枚目 エネミーモデル
	Model* enemy_model_ = nullptr;
	// 02_10 4枚目
	std::list<Enemy*> enemies_;

	// 02_11 15枚目
	DeathParticles* deathParticles_ = nullptr;
	Model* deathParticles_model_ = nullptr;

	// 02_11 16枚目
	Model* deathParticle_model_ = nullptr;

	// --- 攻撃判定デバッグ描画用 ---
	Model* attackAABB_model_ = nullptr;        
	WorldTransform attackAABB_worldTransform_; 
	bool isAttackAABBDrawn_ = false;                         

	// 弾関連
	Model* bullet_model_ = nullptr; // 弾のモデル
	std::list<Bullet*> bullets_;                  // 弾のリスト

	Model* enemy_bullet_model_ = nullptr; // 敵の弾のモデル
	std::list<Bullet*> enemy_bullets_;                  // 敵の弾のリスト

	enum class Phase {
		kIntro, // 登場演出
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

 //ゲームの現在フェーズ（変数）
	    Phase phase_;

	    // 02_12 9枚目
	    void ChangePhase();

		// 02_12 26枚目
	    // クリア時
	    bool finished_ = false;
	    EndStatus endStatus_ = EndStatus::None;

		bool isPaused_ = false; // ポーズ中かどうか

		// 演出タイマー
		float introTimer_ = 0.0f;

		// 演出に書ける時間
	    static inline const float kIntroDuration = 3.0f;

};
