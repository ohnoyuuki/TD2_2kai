#include"KamataEngine.h"
#include"Player.h"
#include"GameScene.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include"MyMath.h"
#define NOMINMAX
#include<algorithm>
#include"MapChipField.h"
using namespace KamataEngine;
using namespace MathUtility;



void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position) 
{
	//NULLポインタチェック
	assert(model); 

	//引数の内容をメンバ変数に記録
	camera_ = camera;

	model_ = model;
	
   


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	
	//初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}




void Player::Update() 
{ 
	
	



	#pragma region 移動入力
	/*
	if (onGround_)
	{
		//左右加速
		KamataEngine::Vector3 acceleration = {};


		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_UP))
		{

			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				// 左移動中の右入力
				if (velocity_.x < 0.0f)
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight)
				{
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
			{
				// 右移動中の左入力
				if (velocity_.x > 0.0f) 
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
				velocity_.x *= (1.0f - kAcceleration);

				if (lrDirection_ != LRDirection::kLeft)
				{
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}

			


			// 加速/減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);


		} else 
		{
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}


		if (Input::GetInstance()->PushKey(DIK_UP)) 
		{
			// ジャンプ加速
			velocity_ += KamataEngine::Vector3(0, kJumpAcceleration, 0);
		}

	}else // 空中
	{
		// 落下速度
		velocity_ += KamataEngine::Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = max(velocity_.y, -kLimitFallSpeed);
	}
		
		
		
	// 着地フラグ
	bool landing = false;
		// 地面との当たり判定
		// 下降中？
	if (velocity_.y < 0) 
	{
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) 
		{
			landing = true;
		}
	}


	if (onGround_)
	{
		// ジャンプ開始
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
	} else 
	{
		// 着地
		if (landing) 
		{
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
	}
	*/	
	#pragma endregion	


	#pragma region 旋回制御
	/*
	if (turnTimer_ > 0.0f)
	{
		//旋回タイマーを1/60秒だけカウントダウンする
		turnTimer_ -= 1.0f / 60.0f;
	
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] =
		{
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	
	
	
	}
	*/
	#pragma endregion

    // 1 移動入力
	InputMove();


	// 2 移動量を加味して衝突判定する
	
	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	//マップ衝突チェック
	//CheckMapCollision(collisionMapInfo);
	
	
	// 3 判定結果を反映して移動させる
	//移動
	//worldTransform_.translation_ += velocity_;
	CheckMapMove(collisionMapInfo);
	



	// 4 天井に接触している場合の処理
	CheckMapCelling(collisionMapInfo);




	// 5 壁に接触している場合の処理

	// 6 設置状態の切り替え
		
	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0)
	{
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f)
		{
			landing = true;
		}
	}

	//設置判定
	if (onGround_)
	{
		// ジャンプ開始
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing)
		{
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
	}
	// 7 旋回制御
	AnimationTurn();
	// 8 アフィン変換行列の作成(プレイヤーの位置)
	worldTransform_.matWorld_ = MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();


}








void Player::Draw()
{ 
	model_->Draw(worldTransform_, *camera_);
	//model_->Draw(worldTransform_, *camera_, textureHandle_);
}








// 移動入力
void Player::InputMove()
{
	//設置状態
	if (onGround_) 
	{
		// 左右加速
		KamataEngine::Vector3 acceleration = {};

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
		{

			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				// 左移動中の右入力
				if (velocity_.x < 0.0f)
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight)
				{
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
			{
				// 右移動中の左入力
				if (velocity_.x > 0.0f)
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
				velocity_.x *= (1.0f - kAcceleration);

				if (lrDirection_ != LRDirection::kLeft)
				{
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速/減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else
		{
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			// ジャンプ加速
			velocity_ += KamataEngine::Vector3(0, kJumpAcceleration, 0);
		}

	} else // 空中
	{
		// 落下速度
		velocity_ += KamataEngine::Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = max(velocity_.y, -kLimitFallSpeed);
	}
}




//マップ衝突チェック
void Player::CheckMapCollision(CollisionMapInfo& info)
{
	CheckMapCollisionUp(info);
	// CheckMapCollisionDown(info);
	// CheckMapCollisionRight(info);
	// CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info)
{


	//上昇あり？
	if (info.move.y <= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<KamataEngine::Vector3, kNumCorner> poditionsNew;

	for (uint32_t i = 0; i < poditionsNew.size(); i++)
	{
		poditionsNew[i] = CornerPosition
		(
			worldTransform_.translation_ + info.move, 
			static_cast<Corner>(i)
		);
	}

	MapChipType mapChipType;
	//真上の当たり判定を行う
	bool hit = false;
	
	
	//左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(poditionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}


	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(poditionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}



	

	//ブロックにヒット？
	if (hit)
	{
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex)
		{
			// めり込みブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		    //天井に当たったことを記録する
			info.celling = true;
		
		}
	}
}

void Player::CheckMapMove(CollisionMapInfo& info)
{
	//移動
	worldTransform_.translation_ + info.move;
}

void Player::CheckMapCelling(CollisionMapInfo& info)
{
	//天井に当たった
	if (info.celling)
	{
		DebugText::GetInstance()->ConsolePrintf("hit cerring\n");
		velocity_.y = 0;
	}
}



//旋回制御
void Player::AnimationTurn()
{
	if (turnTimer_ > 0.0f) 
	{
		// 旋回タイマーを1/60秒だけカウントダウンする
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}


KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{ 
	KamataEngine::Vector3 offsetTable[kNumCorner] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},//kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},//kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},//kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];

	
}
