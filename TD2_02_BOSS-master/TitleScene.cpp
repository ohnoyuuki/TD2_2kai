#include"TitleScene.h"
#include "Player.h"
#include "MapChipField.h"

using namespace KamataEngine;

void TitleScene::Initialize()
{
	//タイトルのスプライト
	textureHandle_ = TextureManager::Load("title.png");
	titleSprite_ = Sprite::Create(textureHandle_, {0, 0});



	// 3Dモデルの生成
	//model_ = Model::CreateFromOBJ("titleFont");
	// modelPlayer_ = Model::CreateFromOBJ("player");
	
	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() 
{

	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finished_ = true;
		}
		if (Input::GetInstance()->PushKey(DIK_T)) 
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finished2_ = true;
		}
		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finished_ = true;
		}
		break;
	}
}

void TitleScene::Draw() 
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// ここに3Dモデルインスタンスの描画処理を記述する
	//model_->Draw(worldTransform_, camera_);
	// modelPlayer_->Draw(worldTransformPlayer_, camera_);


	// 3Dモデル描画後処理
	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());


	titleSprite_->Draw();


	Sprite::PostDraw();



	// フェード
	fade_->Draw();
}

TitleScene::~TitleScene()
{
	// モデル
	//delete model_;
	// delete modelPlayer_;
	//  フェード
	delete fade_;
	//タイトルのスプライト
	delete titleSprite_;
}