#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CameraController.h"
#include "DeathParticle.h"
#include"Fade.h"
#include <vector>
// ゲームシーン




class GameScene
{
public:

	// デストラクタ
	~GameScene();
	
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;



	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// デバックカメラの生成
	// debugCamera_ = new DebugCamera();


	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;

	// キューブ
	Skydome* skydome_ = nullptr;

	// 3D
	KamataEngine::Model* cube_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;
	// モデルプレイヤー
	KamataEngine::Model* modelPlayer_ = nullptr;
	


	// 天球
	KamataEngine::Model* modelskydome_ = nullptr;
	
	// マップチップフィールド
	MapChipField* mapChipField_;
	
	// 表示ブロックの生成
	void GenerateBlocks();
	
	// カメラコントロール
	CameraController* cameraController_ = nullptr;
	// void Initialize();
	// KamataEngine::Model* cameraModel_;
	
	// 敵
	KamataEngine::Model* modelEnemy_ = nullptr;
	// 敵
	// Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	
	// 全ての当たり判定
	void CheckAllCollisions();

	// パーティクル
	KamataEngine::Model* modelParticle_ = nullptr;

	// モデルパーティクル
	DeathParticle* deathParticles_ = nullptr;

	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn, // フェードイン
		kPlay,   // ゲームプレイ
		kDeath,  // デス演出
		kFadeOut,// フェードアウト
	};

	// ゲームの現在フェーズから開始
	Phase phase_;
	
	// フェーズの切り替え
	void ChangePhase();
	
	// 終了フラグ
	bool finished_ = false;
	
	// フェード
	Fade* fade_ = nullptr;

	
};
