#include "GameOver.h"

#include "MapChipField.h"


using namespace KamataEngine;

void GameOver::Initialize()
{
	textureHandle_ = TextureManager::Load("gameover.png");
	overSprite_ = Sprite::Create(textureHandle_, {0, 0});



	
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

void GameOver::Update()
{
	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->PushKey(DIK_E))
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedO_ = true;
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
			finishedO_ = true;
		}
		break;
	}
}

void GameOver::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	

	Sprite::PreDraw(dxCommon->GetCommandList());

	overSprite_->Draw();

	Sprite::PostDraw();


	// フェード
	fade_->Draw();
}

GameOver::~GameOver() 
{
	
	//  フェード
	delete fade_;
	delete overSprite_;
}
