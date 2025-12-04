#include <Windows.h>
#include"KamataEngine.h"
#include"TitleScene.h"
#include"Tutorial.h"
#include"GameScene.h"
#include"GameClear.h"
#include"GameOver.h"


using namespace KamataEngine;

// DirectXCommonインスタンスの取得
DirectXCommon* dxCommon = DirectXCommon::GetInstance();

enum class Scene
{
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



	//ImGuiManagerインスタンスの取得
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


	while (true)
	{
		// エンジンの更新
		if (KamataEngine::Update())
		{
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
	delete tutorial;

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

void UpdateScene() 
{
	switch (scene)
	{
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

void ChangeScene() 
{
	switch (scene) 
	{
	case Scene::kTitle:
		if (titleScene->IsFinished()) 
		{
			// シーンの変更
			scene = Scene::kGame;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}

		if (titleScene->IsFinished2()) 
		{
			// シーンの変更
			scene = Scene::kTutorial;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			tutorial = new Tutorial();
			tutorial->Initialize();
		}

		break;
	case Scene::kTutorial:
		if (tutorial->IsFinished())
		{
			// シーンの変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete tutorial;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	case Scene::kGame:

		//ゲームシーンでバリアが破壊された場合
		if (gameScene->IsFinished())
		{
			// シーンの変更
			scene = Scene::kOver;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;

			gameOver = new GameOver;
			gameOver->Initialize();
		}

		//プレイヤーが敵を倒した場合
		if (gameScene->IsFinished2())
		{
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

		if (gameClear->IsFinished())
		{
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

		if (gameOver->IsFinished())
		{
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

void DrawScene()
{

	switch (scene) 
	{
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