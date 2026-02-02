#include "TitleScene.h"
#include "RuleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOver.h"
#include <KamataEngine.h>
#include <Windows.h>

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
RuleScene* ruleScene = nullptr;
GameScene* gameScene = nullptr;
ClearScene* clearScene = nullptr;
GameOver* gameOver = nullptr;

enum Scene {
	kUnKnown = 0,
	kTitle,
	kRule,
	kGame,
	kGameClear,
	kGameOver,
};

Scene scene = Scene::kUnKnown;


void ChangeScene() {
	switch (scene) {

	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kRule;
			delete titleScene;

			titleScene = nullptr;
			ruleScene = new RuleScene();
			ruleScene->Initialize();
		}
		break;

	case Scene::kRule:
		if (ruleScene->IsFinished()) {
			scene = Scene::kGame;
			delete ruleScene;

			titleScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;

		
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			// 終了ステータスの取得
			GameScene::EndStatus status = gameScene->GetEndStatus();

			// それぞれのステータスによって次のシーンへ準備
			if (status == GameScene::EndStatus::GameClear) {
				// 1,ゲームクリアの場合
				delete gameScene;
				gameScene = nullptr;

				scene = Scene::kGameClear;
				clearScene = new ClearScene();
				clearScene->Initialize();

			} else if (status == GameScene::EndStatus::GameOver) {
				// 2,ゲームオーバーの場合
				delete gameScene;
				gameScene = nullptr;

				scene = Scene::kGameOver;
				gameOver = new GameOver();
				gameOver->Initialize();

			} else if (status == GameScene::EndStatus::Reset) {
				// 3,リセットの場合
				delete gameScene;
				gameScene = nullptr;

				scene = Scene::kGame; // シーンは kGame
				gameScene = new GameScene();
				gameScene->Initialize();
			}
		}

		// タイトルに戻る場合
		if (Input::GetInstance()->TriggerKey(DIK_T) && Input::GetInstance()->TriggerKey(DIK_I)) {
			delete gameScene;
			gameScene = nullptr;

			scene = Scene::kTitle;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}

		break;

	case Scene::kGameClear:
		if (clearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete clearScene;

			clearScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	case Scene::kGameOver:
		if (gameOver->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOver;

			gameOver = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kRule:
		ruleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameClear:
		clearScene->Update();
		break;
	case Scene::kGameOver:
		gameOver->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kRule:
		ruleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameClear:
		clearScene->Draw();
		break;
	case Scene::kGameOver:
		gameOver->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2D_24_フルヤ_ユウト_桃鬼喧嘩");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
		ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	scene = Scene::kTitle;

	titleScene = new TitleScene();
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

	    imguiManager->Begin();

		UpdateScene();

		imguiManager->End();

		dxCommon->PreDraw();

		ChangeScene();

		DrawScene();

		dxCommon->PostDraw(); 
	}

	

	// エンジンの終了処理
	Finalize();

	return 0;
}
