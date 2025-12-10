#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "Tutorial.h"
#include <Windows.h>

using namespace KamataEngine;

// DirectXCommonインスタンスの取得
DirectXCommon* dxCommon = DirectXCommon::GetInstance();

enum class Scene {
	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
	kOver,
};
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();

// 音声ファイル
uint32_t soundTitleHandle_ = 0;
uint32_t soundGameHandle_ = 0;
uint32_t soundClearHandle_ = 0;
uint32_t soundOverHandle_ = 0;

// 再生ハンドル（voice）
int voiceTitleHandle_ = -1;
int voiceGameHandle_ = -1;
int voiceClearHandle_ = -1;
int voiceOverHandle_ = -1;

// 効果音
uint32_t soundBotanHandle_ = 0;

// タイトルシーンの生成
TitleScene* titleScene = nullptr;

// チュートリアルシーンの生成
Tutorial* tutorial = nullptr;

// ゲームシーンのインスタンス生成
GameScene* gameScene = nullptr;

// ゲームクリアシーンの生成
GameClear* gameClear = nullptr;

// ゲームオーバーシーンの生成
GameOver* gameOver = nullptr;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"2265_天空の決戦");

	// サウンドデータの読み込み
	soundTitleHandle_ = Audio::GetInstance()->LoadWave("BossTitle.mp3");
	soundGameHandle_ = Audio::GetInstance()->LoadWave("BossPlay.mp3");
	soundClearHandle_ = Audio::GetInstance()->LoadWave("BossClear.mp3");
	soundOverHandle_ = Audio::GetInstance()->LoadWave("BossOver.mp3");

	// 効果音データの読み込み
	soundBotanHandle_ = Audio::GetInstance()->LoadWave("BossBotan.mp3");

	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	tutorial = new Tutorial;
	gameClear = new GameClear;
	gameOver = new GameOver;

	// タイトルシーンの初期化
	titleScene->Initialize();

	// チュートリアルシーンの初期化
	tutorial->Initialize();

	/*
	//ゲームシーンの初期化
	gameScene->Initialize();
	*/

	gameOver->Initialize();

	gameClear->Initialize();

	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// シーン切り替え
		ChangeScene();

		imguiManager->Begin();

		// 現在シーン更新
		UpdateScene();

		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// タイトルシーンの解放
	delete titleScene;

	// チュートリアルシーンの解放
	if (tutorial) {
		delete tutorial;
	}

	// ゲームシーンの解放
	delete gameScene;

	// ゲームクリアシーンの解放
	delete gameClear;

	// ゲームオーバーシーンの解放
	delete gameOver;

	// nullptrの代入

	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;

	case Scene::kTutorial:
		tutorial->Update();
		break;

	case Scene::kGame:
		gameScene->Update();
		break;

	case Scene::kClear:
		gameClear->Update();
		break;

	case Scene::kOver:
		gameOver->Update();
		break;
	}
}

void ChangeScene() {

	static Scene lastScene = Scene::kUnknown; // 前のシーンを記憶

	if (scene != lastScene) {
		// --- シーン切り替え音声管理 --------------------
		// 全てのBGMを一旦停止
		Audio::GetInstance()->StopWave(soundTitleHandle_);
		Audio::GetInstance()->StopWave(soundGameHandle_);
		Audio::GetInstance()->StopWave(soundClearHandle_);
		Audio::GetInstance()->StopWave(soundOverHandle_);

		// 新しいシーンに応じて再生
		switch (scene) {
		case Scene::kTitle:
			Audio::GetInstance()->PlayWave(soundTitleHandle_, true);
			break;

		case Scene::kTutorial:
			Audio::GetInstance()->PlayWave(soundTitleHandle_, true);
			break;

		case Scene::kGame:
			Audio::GetInstance()->PlayWave(soundGameHandle_, true);
			break;

		case Scene::kClear:
			Audio::GetInstance()->PlayWave(soundClearHandle_, false);
			break;

		case Scene::kOver:
			Audio::GetInstance()->PlayWave(soundOverHandle_, false);
			break;
		}
		// -----------------------------------------------

		lastScene = scene;
	}

	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinishedT()) {
			// シーンの変更
			scene = Scene::kGame;

			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}

		if (titleScene->IsFinishedT2()) {
			// シーンの変更
			scene = Scene::kTutorial;

			/// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			tutorial = new Tutorial();
			tutorial->Initialize();
		}

		break;
	case Scene::kTutorial:
		if (tutorial->IsFinishedTU()) {
			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete tutorial;
			tutorial = nullptr;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	case Scene::kGame:

		// ゲームシーンでバリアが破壊された場合
		if (gameScene->IsFinishedGAME()) {
			// シーンの変更
			scene = Scene::kOver;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			gameOver = new GameOver;
			gameOver->Initialize();
		}

		// プレイヤーが敵を倒した場合
		else if (gameScene->IsFinishedGAME2()) {
			// シーンの変更
			scene = Scene::kClear;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;

			// クリアシーンの生成
			gameClear = new GameClear;
			// クリアシーンの初期化
			gameClear->Initialize();
		}
		break;

	case Scene::kClear:

		if (gameClear->IsFinishedC()) {
			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameClear;
			gameClear = nullptr;

			// タイトルシーンの生成
			titleScene = new TitleScene;
			// タイトルシーンの初期化
			titleScene->Initialize();
		}
		break;

	case Scene::kOver:

		if (gameOver->IsFinishedO()) {
			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameOver;
			gameOver = nullptr;

			// タイトルシーンの生成
			titleScene = new TitleScene;
			// タイトルシーンの初期化
			titleScene->Initialize();
		}
		break;
	}
}

void DrawScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;

	case Scene::kTutorial:
		tutorial->Draw();
		break;

	case Scene::kGame:
		gameScene->Draw();
		break;

	case Scene::kClear:
		gameClear->Draw();
		break;

	case Scene::kOver:
		gameOver->Draw();
		break;
	}
}