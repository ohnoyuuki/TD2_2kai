#include "GameScene.h"
#include "MapChipField.h"
#include "MyMath.h"
#include "CameraController.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Fade.h"
using namespace KamataEngine;



//初期化
void GameScene::Initialize()
{ 
	// h(ヘッターファイル)にいれる

	// textureHandle_ = TextureManager::Load("Fruuits.png");


	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;


	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 3Dモデルの生成
	modelskydome_ = Model::CreateFromOBJ("skydome", true);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(100, 200);
	

	cube_ = Model::CreateFromOBJ("block");

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);


	// 敵の3Dモデルデータの生成
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);


	//パーティクルの3Dモデルデータの生成
	modelParticle_ = Model::CreateFromOBJ("deathParticle", true);




	// 自キャラの生成
	player_ = new Player();
	
	// 敵の生成
	//enemy_ = new Enemy();
	
	
	/**/
	for (int32_t i = 0; i < 3; i++)
	{
		Enemy* newEnemy = new Enemy();
		KamataEngine::Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(32 + i, 16 + i);
	    newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
	
	
	    enemies_.push_back(newEnemy);
	}
    






	//マップチップフィールドの生成
	mapChipField_ = new MapChipField;


	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_); // 自キャラの生成と初期化
	
	

	/**/
	// パーティクル
	deathParticles_ = new DeathParticle();
	deathParticles_->Initialize(modelParticle_, &camera_, playerPosition);
	




	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();



	
	// カメラの初期化
	camera_.Initialize();

	// スカイドームの生成
	skydome_ = new Skydome();

	// スカイドームの初期化
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);








	
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	//表示ブロックの生成
	GenerateBlocks();
	
	
	
	
	

	
	

	// 自キャラの初期化
	// player_->Initialize(modelPlayer_,&camera_,playerPosition);

	
	

	// カメラコントローラの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	// マップチップフィールドの生成と初期化


	//フェーズインから開始
	phase_ = Phase::kFadeIn;
	
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}



//ブロック
void GameScene::GenerateBlocks() 
{
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	// ブロック1個分の横幅
	// const float kBlockWidth = 2.0f;
	// const float kBlockHeight = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	////キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; i++)
	{
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; i++)
	{
		for (uint32_t j = 0; j < numBlockHorizontal; j++) 
		{
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) // 1マス分にボックスの形にしたいなら(i + j)にする
			{
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}



//デストラクタ
GameScene::~GameScene() 
{
	delete sprite_;

	delete skydome_;

	delete player_;

	delete deathParticles_;

	// フェード
	delete fade_;

	for (Enemy* enemy : enemies_)
	{
		delete enemy;
	}

	//delete enemy_;

	// 3Dモデルデータの解放
	delete model_;
	
	//デバッグカメラの解放
	delete debugCamera_;

	// マップチップフィールドの解放
	delete mapChipField_;

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}



//更新
void GameScene::Update() 
{


	// フェード
	fade_->Update();

	switch (phase_)
	{
	case Phase::kPlay:
		
		// 全ての当たり判定
		CheckAllCollisions();

		// ゲームプレイフェーズの処理
		if (player_->IsDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			deathParticles_ = new DeathParticle();
			deathParticles_->Initialize(modelParticle_, &camera_, deathParticlesPosition);
		}		
		break;

	case Phase::kDeath:
		// デス演出フェーズの処理
		
		// デスパーティクルの更新
		deathParticles_->Update();
		if (deathParticles_ && deathParticles_->isFinished_)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}


		/*
		if ("deathParticle", true)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			deathParticles_->Update();
			finished_ = deathParticles_->isFinished_;
		}
		*/

		break;
	case Phase::kFadeIn:
			//フェード
		    fade_->Update();
			if (fade_->IsFinished())
			{
			    phase_ = Phase::kPlay;
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
	
	// 自キャラの更新
	player_->Update();

	// 天球の更新
	skydome_->Update();

	// 敵の更新
	// enemy_->Update();
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}
    
	// カメラコントローラーの更新
	cameraController_->Update();


	// 行列を定義バッファに転送
	// worldTransform_.TransferMatrix();
	

	// debugCamera_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG

	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else 
	{
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		camera_.TransferMatrix();
	}
	// ブロックの更新
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{

			if (!worldTransformBlock)
			{
				continue;
			}

			// アフィン変換行列の作成

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			////定数バッファに転送する

			worldTransformBlock->TransferMatrix();
		}
	}
}



//描画
void GameScene::Draw()
{

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 3Dモデル描画
	//
	// model_->Draw(worldTransform_, camera_, textureHandle_);

	// 自キャラの描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn)
	{
		player_->Draw();
	}



	//パーティクル
	if ("deathParticle", true) 
	{
		deathParticles_->Draw();
	}



	// 敵の描画
	//enemy_->Draw();
	for (Enemy* enemy : enemies_)
	{
		enemy->Draw();
	}

	//ブロックの描画
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{
			if (!worldTransformBlock)
			{
				continue;
			}
			cube_->Draw(*worldTransformBlock, camera_);
		}
	}

	skydome_->Draw();

	// 3Dモデル描画前処理
	Model::PostDraw(); // プログラムの終了

	// フェード
	fade_->Draw();
}



//衝突判定
void GameScene::CheckAllCollisions()
{


#pragma region 自キャラと敵キャラの当たり判定
	
	//判定対象1と2の座標
	AABB aabb1, aabb2;
	
	aabb1 = player_->GetAABB();

	//自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_)
	{
		//敵弾の座標
		aabb2 = enemy->GetAABB();



		//AABB同士の交差判定
		if (IsCollition(aabb1, aabb2))
		{
			//自キャラの衝突時関数を呼び出す
			player_->OnCollition(enemy);
			//敵の衝突時関数を呼び出す
			enemy->OnCollition(player_);
		}
	}




#pragma endregion




}



//フェーズ
void GameScene::ChangePhase()
{

	switch (phase_) 
	{
	case Phase::kPlay:
		// ゲームプレイフェーズの処理

		if (player_->IsDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			deathParticles_ = new DeathParticle();
			deathParticles_->Initialize(modelParticle_, &camera_, deathParticlesPosition);
		}

		break;

	case Phase::kDeath:
		// デス演出フェーズの処理
		

		if (deathParticles_)
		{
			//シーン終了
			finished_ = true;
		}
		
		
		break;
	}
}
