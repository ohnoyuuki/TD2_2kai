#include "GameScene.h"
#include "MyMath.h"
#include "CameraController.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Fade.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Barrier.h"
using namespace KamataEngine;



//初期化
void GameScene::Initialize()
{ 
	// h(ヘッターファイル)にいれる

	// textureHandle_ = TextureManager::Load("Fruuits.png");


	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;


	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	#pragma region 3Dモデル

	//バリアの生成
	modelBarrier_ = Model::CreateFromOBJ("barrier", true);

	// 3Dモデルの生成
	modelskydome_ = Model::CreateFromOBJ("skydome", true);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(100, 200);
	

	


	// 自キャラの生成
	//modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelPlayer_ = Model::CreateFromOBJ("roboto", true);
	
	
	//自キャラの弾
	modelPlayerBullet_ = Model::CreateFromOBJ("tama", true);



	// 敵の3Dモデルデータの生成
	modelEnemy_ = Model::CreateFromOBJ("boss", true);
	// 敵の弾
	modelEnemyBullet_ = Model::CreateFromOBJ("Etama", true);


	//パーティクルの3Dモデルデータの生成
	modelParticle_ = Model::CreateFromOBJ("deathParticle", true);

	#pragma endregion

	#pragma region 初期化
	// 自キャラの生成
	player_ = new Player();
	
	// 敵の生成
	enemy_ = new Enemy();
	
	
	


	//プレイヤー
	// 座標を指定
	KamataEngine::Vector3 playerPosition = {-10, 1, 1};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	//player_->SetMapChipField(mapChipField_); // 自キャラの生成と初期化
	// 自キャラの弾
	playerBullet_ = new PlayerBullet();
	playerBullet_->Initialize(modelPlayerBullet_, &camera_, playerPosition, velocity_);
	
	

	//敵
	KamataEngine::Vector3 enemyPosition = {20, 5, 5};
	enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);
	//enemy_->SetMapChipField(mapChipField_);
	
	// 敵の弾
	enemyBullet_ = new EnemyBullet();
	enemyBullet_->Initialize(modelEnemyBullet_, &camera_, enemyPosition, EnemyBulletVelocity_);
	
	
	
	// デスパーティクル
	deathParticles_ = new DeathParticle();
	deathParticles_->Initialize(modelParticle_, &camera_, playerPosition);
	
	
	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);



	//バリア
	barrier_ = new Barrier();
	barrier_->Initialize(modelBarrier_, textureHandle_, &camera_);
	
	

	// 自キャラの初期化
	// player_->Initialize(modelPlayer_,&camera_,playerPosition);


	// カメラコントローラの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	
#pragma endregion

	//フェーズインから開始
	phase_ = Phase::kFadeIn;
	


	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}



//ブロック
//void GameScene::GenerateBlocks() 
//{
//	// 要素数
//	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
//	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
//	// ブロック1個分の横幅
//	// const float kBlockWidth = 2.0f;
//	// const float kBlockHeight = 2.0f;
//	// 要素数を変更する
//	worldTransformBlocks_.resize(numBlockVirtical);
//
//	////キューブの生成
//	for (uint32_t i = 0; i < numBlockVirtical; i++)
//	{
//		worldTransformBlocks_[i].resize(numBlockHorizontal);
//	}
//
//	// ブロックの生成
//	for (uint32_t i = 0; i < numBlockVirtical; i++)
//	{
//		for (uint32_t j = 0; j < numBlockHorizontal; j++) 
//		{
//			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) // 1マス分にボックスの形にしたいなら(i + j)にする
//			{
//				WorldTransform* worldTransform = new WorldTransform();
//				worldTransform->Initialize();
//				worldTransformBlocks_[i][j] = worldTransform;
//				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
//			}
//		}
//	}
//}



//デストラクタ
GameScene::~GameScene() 
{
	delete sprite_;

	delete skydome_;

	delete player_;
	
	delete enemy_;

	delete barrier_;

	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}

	for (EnemyBullet* Ebullet : E_bullets_)
	{
		delete Ebullet;
	}


	delete deathParticles_;
	
	// フェード
	delete fade_;


	// 3Dモデルデータの解放
	delete model_;
	
	//デバッグカメラの解放
	delete debugCamera_;

	
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


		if (enemy_->IsEnemyDead() == true) 
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kEnemyDeath;
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
			finishedGAME_ = true;
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

case Phase::kEnemyDeath:

		// デスパーティクルの更新
		deathParticles_->Update();
		if (deathParticles_ && deathParticles_->isFinished_)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedGAME2_ = true;
		}
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
			    finishedGAME_ = true;
			    
			} else
			{
			    finishedGAME2_ = true;
			}
		    break;
	}
	
	// カメラコントローラーの更新
	cameraController_->Update();




	// 天球の更新
	skydome_->Update();





	// 自キャラの更新
	player_->Update();
	//自キャラの攻撃を呼び出す
	PlayerAttack();
	
	//自キャラの弾を更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}



	// 敵の更新
	enemy_->Update();
	EnemyAttack();

	
	// 敵の弾を更新
	for (EnemyBullet* Ebullet : E_bullets_)
	{
		Ebullet->Update();
	}


	// バリア
	barrier_->Update();



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

	// 自キャラの描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kEnemyDeath)
	{
		player_->Draw();
	}

	
	// 敵の描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kDeath) 
	{
		enemy_->Draw();
	}

	


	

//パーティクル
	if ("deathParticle", true) 
	{
		deathParticles_->Draw();
	}
	
	

	#pragma region 自キャラの弾の処理

	// スペースキーを押して弾を撃つ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) 
	{
		playerBulletLifeTime--;
	}

	// 弾の継続時間が0になるまで撃てる
	if (playerBulletLifeTime > 0)
	{
		for (PlayerBullet* bullet : bullets_) 
		{
			bullet->Draw();
		}
	}

	//弾の継続時間が0になったら継続時間をリセットする
	if (playerBulletLifeTime <= 0) 
	{
		//delete playerBullet_;
		bullets_.clear();
		playerBulletLifeTime = 20;
	}

	#pragma endregion

	


	
	
	#pragma region 敵の弾の処理

	
	
	
	// 弾の継続時間が0になるまで表示
	
	
	
	





	
	enemyBulletLifeTime--;
	// 弾の継続時間が0になるまで撃てる
	if (enemyBulletLifeTime > 0)
	{
		for (EnemyBullet* Ebullet : E_bullets_)
		{
			Ebullet->Draw();
		}
	}

	// 弾の継続時間が0になったら継続時間をリセットする
	if (enemyBulletLifeTime <= 0) 
	{

		E_bullets_.clear();
		enemyBulletLifeTime = 200;
	}






#pragma endregion



	//バリア
	barrier_->Draw();


	/*
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
	}*/
    




	skydome_->Draw();



	// 3Dモデル描画前処理
	Model::PostDraw(); // プログラムの終了

	// フェード
	fade_->Draw();
}



//衝突判定
void GameScene::CheckAllCollisions()
{


#pragma region 当たり判定
	
	// プレイヤー     AABB1
	
	// 敵             AABB2
	
	//プレイヤーの弾  AABB3
	
	//敵の弾          AABB4

	//バリア          AABB5



	
	
	#pragma region 自キャラと敵
	//判定対象1と2の座標
	AABB aabb1, aabb2;
	//  プレイヤー
	aabb1 = player_->GetAABB();
	// 敵
	aabb2 = enemy_->GetAABB();
	// AABB同士の交差判定
	if (IsCollition(aabb1, aabb2))
	{
		// 自キャラの衝突時関数を呼び出す
		player_->OnCollition(enemy_);
		// 敵の衝突時関数を呼び出す
		enemy_->OnCollition(player_);
	}
	#pragma endregion



	#pragma region 自キャラの弾と敵
	// 判定対象1と2の座標
	AABB2 aabb3, aabb4;
	// 敵
	aabb3 = enemy_->GetAABB2();
	
	
	for (PlayerBullet* bullet : bullets_)
	{
		//自キャラの弾
		aabb4 = bullet->GetAABB2();
		if (IsCollition2(aabb3, aabb4))
		{
			// 自キャラの衝突時関数を呼び出す
  			bullet->OnCollition2(enemy_);
			// 敵の衝突時関数を呼び出す
			enemy_->OnCollition2(bullet);
		}
	}
	#pragma endregion
	

	#pragma region 敵の弾とプレイヤーの弾

	// 判定対象1と2の座標
	AABB3 aabb5, aabb6;
	
	for (PlayerBullet* bullet : bullets_)
	{
		// 自キャラの弾
		aabb5 = bullet->GetAABB3();
		if (IsCollition3(aabb5, aabb6))
		{
			// 自キャラの衝突時関数を呼び出す
			bullet->OnCollition3(enemyBullet_);
			// 敵の衝突時関数を呼び出す
			enemyBullet_->OnCollition3(bullet);
		}
	}

	for (EnemyBullet* Ebullet : E_bullets_)
	{
		// 自キャラの弾
		aabb6 = Ebullet->GetAABB3();
		if (IsCollition3(aabb5, aabb6))
		{
			// 自キャラの衝突時関数を呼び出す
			Ebullet->OnCollition3(playerBullet_);
			// 敵の衝突時関数を呼び出す
			playerBullet_->OnCollition3(Ebullet);
		}
	}
	#pragma endregion







#pragma endregion




}


//自キャラの攻撃
void GameScene::PlayerAttack() 
{
	//スペースキーを押して弾を撃つ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {kBulletSpeed, 0.0f, 0.0f};


		// 自キャラの座標を取得(弾を自キャラと同じ位置にする)
		const KamataEngine::Vector3 playerBulletPosition = player_->GetWorldPosition();

		
		playerBullet_ = new PlayerBullet();
		playerBullet_->Initialize(modelPlayerBullet_, &camera_, playerBulletPosition, velocity);


		bullets_.push_back(playerBullet_);

	}
}

void GameScene::EnemyAttack()
{

	// 弾の速度
	const float kEBulletSpeed = 1.0f;
	Vector3 E_bulletVelocity = {kEBulletSpeed, 0.0f, 0.0f};

	// 座標を取得(弾を自キャラと同じ位置にする)
	const KamataEngine::Vector3 enemyBulletPosition = enemy_->GetWorldPosition();

	enemyBullet_ = new EnemyBullet();
	enemyBullet_->Initialize(modelEnemyBullet_, &camera_, enemyBulletPosition, E_bulletVelocity);

	E_bullets_.push_back(enemyBullet_);
}



//フェーズ
void GameScene::ChangePhase()
{

	switch (phase_) 
	{
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		#pragma region プレイヤー
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
        #pragma endregion


		#pragma region 敵
		if (enemy_->IsEnemyDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kEnemyDeath;

			
		}
		#pragma endregion



		break;

	case Phase::kDeath:
		// デス演出フェーズの処理
		

		if (deathParticles_)
		{
			//シーン終了
			finishedGAME_ = true;
		}

		break;
	case Phase::kEnemyDeath:

		
		// シーン終了
		finishedGAME2_ = true;
		
			
		
		
		break;
	}
}
