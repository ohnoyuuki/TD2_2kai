#include"KamataEngine.h"
#include"Player.h"
#include"GameScene.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include<list>
#define NOMINMAX
#include"MapChipField.h"


using namespace KamataEngine;
using namespace MathUtility;



void Player::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;

	camera_ = camera;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	worldTransform_.Initialize();
}

void Player::Update()
{
	// 1.移動入力
	InputMove();
	// 2.移動量を加速して衝突判定する
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	// 3.判定結果を反映して移動させる
	CheckMapMove(collisionMapInfo);
	// 4.天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);
	// 5.壁に接触している場合の処理
	CheckMapWall(collisionMapInfo);
	// 6.接地状態の切り替え
	CheckMapLanding(collisionMapInfo);
	// 7.旋回制御
	AnimateTurn();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	Vector3 acceleration = {};

	// 押した方向で移動ベクトルを変更(左右)
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		move.x -= kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_D))
	{
		move.x += kCharacterSpeed;
		if (velocity_.x < 0.0f) 
		{
			// 速度と逆方向に入力中は急ブレーキ
			velocity_.x *= (1.0f - kAttenuation);
		}
		acceleration.x += kAccleration;
		if (lrDirection_ != LRDirection::kRight)
		{
			lrDirection_ = LRDirection::kRight;
			// 旋回開始時の角度を記録する
			trunFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマーに時間を設定する
			trunTimer_ = kTimeTurn;
		}
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_W)) 
	{
		move.y += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_S))
	{
		move.y -= kCharacterSpeed;
	}





	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // プレイヤーの座標の計算
}

// プレイヤーの描画(敵当たったら非表示になる)
void Player::Draw() 
{
	if (isDead_) 
	{
		return;
	}

	
	model_->Draw(worldTransform_, *camera_);
}

// 移動入力
void Player::InputMove() 
{

	/*

	// 左右移動操作
	if (onGround_)
	{
	    if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A))
	    {

	        // 左右加速
	        Vector3 acceleration = {};
	        if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D))
	        {
	            // 左移動中の右入力
	            if (velocity_.x < 0.0f)
	            {
	                // 速度と逆方向に入力中は急ブレーキ
	                velocity_.x *= (1.0f - kAttenuation);
	            }
	        	acceleration.x += kAccleration;
	            if (lrDirection_ != LRDirection::kRight)
	            {
	                lrDirection_ = LRDirection::kRight;
	                // 旋回開始時の角度を記録する
	                trunFirstRotationY_ = worldTransform_.rotation_.y;
	                // 旋回タイマーに時間を設定する
	                trunTimer_ = kTimeTurn;
	            }
	        } else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A))
	        {
	            // 右移動中の左入力
	            if (velocity_.x > 0.0f)
	            {
	                // 速度と逆方向に入力中は急ブレーキ
	                velocity_.x *= (1.0f - kAttenuation);
	            }
	            acceleration.x -= kAccleration;
	            if (lrDirection_ != LRDirection::kLeft)
	            {
	                lrDirection_ = LRDirection::kLeft;
	                // 旋回開始時の角度を記録する
	                trunFirstRotationY_ = worldTransform_.rotation_.y;
	                // 旋回タイマーに時間を設定する
	                trunTimer_ = kTimeTurn;
	            }
	        }


	        ////////////////
	        ////////

	        if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W))
	        {
	            // 左移動中の右入力
	            if (velocity_.y < 0.0f)
	            {
	                // 速度と逆方向に入力中は急ブレーキ
	                velocity_.y *= (1.0f - kAttenuation);
	            }
	            acceleration.y -= kAccleration;

	        } else if (Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_S))
	        {
	            // 右移動中の左入力
	            if (velocity_.y > 0.0f)
	            {
	                // 速度と逆方向に入力中は急ブレーキ
	                velocity_.y *= (1.0f - kAttenuation);
	            }
	            acceleration.y += kAccleration;

	        }

	        ///////////
	        ////////////////////////






	        // 加速/減速
	        velocity_ += acceleration;
	        // 最大速度制限
	        velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

	    } else
	    {
	        // 非入力時は移動減衰をかける
	        velocity_.x *= (1.0f - kAccleration);
	    }
	    if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_SPACE))
	    {
	        // ジャンプ初速
	        velocity_ += Vector3(0, kJumpAcceleration, 0);
	    }
	}
	// 空中
	else
	{

	    // 左右加速
	    Vector3 acceleration = {};
	    if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D))
	    {
	        // 左移動中の右入力
	        if (velocity_.x < 0.0f)
	        {
	            // 速度と逆方向に入力中は急ブレーキ
	            velocity_.x *= (1.0f - kAttenuation);
	        }
	        acceleration.x += kAccleration;
	        if (lrDirection_ != LRDirection::kRight)
	        {
	            lrDirection_ = LRDirection::kRight;
	            // 旋回開始時の角度を記録する
	            trunFirstRotationY_ = worldTransform_.rotation_.y;
	            // 旋回タイマーに時間を設定する
	            trunTimer_ = kTimeTurn;
	        }
	    } else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A))
	    {
	        // 右移動中の左入力
	        if (velocity_.x > 0.0f)
	        {
	            // 速度と逆方向に入力中は急ブレーキ
	            velocity_.x *= (1.0f - kAttenuation);
	        }
	        acceleration.x -= kAccleration;
	        if (lrDirection_ != LRDirection::kLeft)
	        {
	            lrDirection_ = LRDirection::kLeft;
	            // 旋回開始時の角度を記録する
	            trunFirstRotationY_ = worldTransform_.rotation_.y;
	            // 旋回タイマーに時間を設定する
	            trunTimer_ = kTimeTurn;
	        }
	    }
	    // 加速/減速
	    velocity_ += acceleration;
	    // 最大速度制限
	    velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);


	    // 落下速度
	    velocity_ += Vector3(0, -kGravityAcceleration, 0);
	    // 落下速度制限
	    velocity_.y = max(velocity_.y, -kLimitFallSpeed);
	}*/
}

// 2.マップ衝突チェック
void Player::CheckMapCollision(CollisionMapInfo& info) 
{
	CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

// マップ衝突チェック　上
void Player::CheckMapCollisionUP(CollisionMapInfo& info) 
{
	// 上昇あり?
	if (info.move.y <= 0) 
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}
	////右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット
	if (hit) 
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

// マップ衝突チェック 下
void Player::CheckMapCollisionDown(CollisionMapInfo& info) 
{
	// 下降あり?
	if (info.move.y >= 0)
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}
	////右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit)
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// 現在座標が壁の外が判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));

		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.langing = true;
	}

	// 着地フラグ
	if (info.langing) 
	{
		// 着地状態に切り替える(落下を止める)
		onGround_ = true;
		// 着地時にX速度を減衰
		velocity_.x *= (1.0f - kAttenuationLanding);
		// Y速度をゼロにする
		velocity_.y = 0.0f;
	}
}

// マップ衝突チェック 右
void Player::CheckMapCollisionRight(CollisionMapInfo& info)
{
	// 右移動あり?
	if (info.move.x <= 0)
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真右の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}
	////右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit)
	{
		// 現在座標が壁の外が判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitwall = true;
	}
}

// マップ衝突チェック 左
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) 
{
	// 左移動あり?
	if (info.move.x >= 0)
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真左の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) 
	{
		hit = true;
	}
	////左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit) 
	{

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitwall = true;
	}
}

// 3.判定結果を反映して移動させる
void Player::CheckMapMove(const CollisionMapInfo& info) 
{
	// 移動
	worldTransform_.translation_ += info.move;
}

// 4.天井に接触している場合の処理
void Player::CheckMapCeiling(const CollisionMapInfo& info)
{
	if (info.ceiling) 
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

// 5.壁に接触している場合の処理
void Player::CheckMapWall(CollisionMapInfo& info) 
{
	// 壁接触による減速
	if (info.hitwall)
	{
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

// 6.接地状態の切り替え処理
void Player::CheckMapLanding(const CollisionMapInfo& info) 
{
	// 自キャラが接地状態
	if (onGround_)
	{
		// 接地状態の処理

		// ジャンプ開始
		if (velocity_.y > 0.0f) 
		{
			onGround_ = false;
		} else
		{
			// 落下判定
			//  移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); i++) 
			{
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定

			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				hit = true;
			}
			////右上点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) 
			{
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit) 
			{
				// 空中状態の処理
				onGround_ = false;
			}
		}
	} else 
	{
		// 空中状態の処理
		if (info.langing) 
		{
			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

// 7.旋回制御
void Player::AnimateTurn()
{
	// 旋回制御
	if (trunTimer_ > 0.0f)
	{
		trunTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, trunFirstRotationY_, trunTimer_ / kTimeTurn);
	}
}


Player::~Player(){}




KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{
	KamataEngine::Vector3 offsetTable[kNumCorner] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

KamataEngine::Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Player::OnCollition(const Enemy* enemy)
{
	(void)enemy;

	// デスフラグを立てる
	isDead_ = true;

	// ジャンプ開始
	// velocity_ += KamataEngine::Vector3(0, kJumpAcceleration, 0);
}