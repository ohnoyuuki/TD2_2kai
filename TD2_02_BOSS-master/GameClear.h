#pragma once
#include"KamataEngine.h"
#include "MapChipField.h"
#include"Fade.h"
class GameClear
{
public:
	enum class Phase
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedC_ = false;
	// デスフラグのgetter
	bool IsFinishedC() const { return finishedC_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~GameClear();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* clearSprite_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// モデルプレイヤー
	// KamataEngine::Model* modelPlayer_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// 音声ファイル
	uint32_t soundTitleHandle_ = 0;
	uint32_t soundGameHandle_ = 0;
	uint32_t soundClearHandle_ = 0;
	uint32_t soundOverHandle_ = 0;

	// 音声再生ハンドル
	int voiceTitleHandle_ = 0;
	int voiceGameHandle_ = 0;
	int voiceClearHandle_ = 0;
	int voiceOverHandle_ = 0;

	// 効果音ハンドル
	uint32_t soundBotanHandle_ = 0;

};
