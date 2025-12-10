#include "GameClear.h"

#include "MapChipField.h"

using namespace KamataEngine;

void GameClear::Initialize() {
	textureHandle_ = TextureManager::Load("CLEAR.png");
	clearSprite_ = Sprite::Create(textureHandle_, {0, 0});

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// サウンドデータの読み込み

	//soundClearHandle_ = Audio::GetInstance()->LoadWave("BossClear.mp3");

	//// 効果音データの読み込み
	//soundBotanHandle_ = Audio::GetInstance()->LoadWave("BossBotan.mp3");

	//// --- 再生ハンドルは全部初期化しておく ---

	//voiceClearHandle_ = -1;

	
}

void GameClear::Update() {
	switch (phase_) {
	case Phase::kMain:

		// タイトルシーンの終了条件
		// タイトルシーンの終了条件
		if (Input::GetInstance()->PushKey(DIK_E)) {
			// 音声停止
			//Audio::GetInstance()->StopWave(voiceClearHandle_);
			// 音声再生
			Audio::GetInstance()->PlayWave(soundBotanHandle_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedC_ = true;
		}

		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) {
			finishedC_ = true;
		}
		break;
	}
}

void GameClear::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 3Dモデル描画後処理
	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	clearSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

GameClear::~GameClear() {

	//  フェード
	delete fade_;
	delete clearSprite_;
}
