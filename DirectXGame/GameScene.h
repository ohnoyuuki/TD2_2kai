#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CameraController.h"
#include "DeathParticle.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Barrier.h"
#include "Fade.h"
#include <vector>
#include <list>
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

	bool IsFinished2() const { return finished2_; }



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

	

	

	// 3D
	KamataEngine::Model* cube_ = nullptr;



	// 自キャラ
	Player* player_ = nullptr;
	// モデルプレイヤー
	KamataEngine::Model* modelPlayer_ = nullptr;
	


	// 敵
	Enemy* enemy_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
	




	#pragma region 自キャラの弾
	//自キャラの弾
	KamataEngine::Model* modelPlayerBullet_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;
	// 速度
	KamataEngine::Vector3 velocity_;
    // 弾の寿命(フレーム数)
    int playerBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ
    
	#pragma endregion 





	#pragma region 敵の弾
	// 敵の弾
	KamataEngine::Model* modelEnemyBullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> E_bullets_;
	// 速度
	KamataEngine::Vector3 EnemyBulletVelocity_;
	// 弾の寿命(フレーム数)
	int enemyBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ
	

	
    #pragma endregion 



	//バリア
	Barrier* barrier_ = nullptr;
	KamataEngine::Model* modelBarrier_ = nullptr;


	
	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelskydome_ = nullptr;
	



	// マップチップフィールド
	MapChipField* mapChipField_;
	
	// 表示ブロックの生成
	void GenerateBlocks();
	
	// カメラコントロール
	CameraController* cameraController_ = nullptr;
	// void Initialize();
	// KamataEngine::Model* cameraModel_;
	
	
	//std::list<Enemy*> enemies_;
	
	// 全ての当たり判定
	void CheckAllCollisions();

	// パーティクル
	KamataEngine::Model* modelParticle_ = nullptr;

	// モデルパーティクル
	DeathParticle* deathParticles_ = nullptr;

	// 自キャラの弾
	PlayerBullet* playerBullet_ = nullptr;
	void PlayerAttack();
	
	// 敵の弾
	EnemyBullet* enemyBullet_ = nullptr;
	void EnemyAttack();





	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn, // フェードイン
		kPlay,   // ゲームプレイ
		kDeath,  // デス演出
		kEnemyDeath,
		kFadeOut,// フェードアウト
	};

	// ゲームの現在フェーズから開始
	Phase phase_;
	
	// フェーズの切り替え
	void ChangePhase();
	
	// 終了フラグ
	bool finished_ = false;
	
	//敵を倒した場合のフラグ
	bool finished2_ = false;


	// フェード
	Fade* fade_ = nullptr;

	
};
