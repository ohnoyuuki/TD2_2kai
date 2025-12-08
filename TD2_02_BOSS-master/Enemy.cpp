#include"Enemy.h"
#include"GameScene.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include<list>
#define NOMINMAX
#include "MapChipField.h"

using namespace KamataEngine;
using namespace MathUtility;




void Enemy::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;

	camera_ = camera;

	
	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
	enemyHp = 1000;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	worldTransform_.Initialize();


	/////////////////////////////////////////
	////敵左右移動追加////////////////////////
	/////////////////////////////////////////
	startX_ = position.x;

	// 最初の「次の行動までの時間」を設定（2〜5秒）
	nextActionTime_ = (rand() % 300) / 60.0f + 2.0f;
	////////////////////////////////////////////
	////敵左右移動追加終///////////////////////
	/////////////////////////////////////////////


}



void Enemy::Update()
{

	
	// 1.移動入力
	
	
	
	//InputMove();
	
	
	
	// 2.移動量を加速して衝突判定する
	// 衝突情報を初期化
	//CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	//collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	//CheckMapCollision(collisionMapInfo);
	// 3.判定結果を反映して移動させる
	//CheckMapMove(collisionMapInfo);
	// 4.天井に接触している場合の処理
	//CheckMapCeiling(collisionMapInfo);
	// 5.壁に接触している場合の処理
	//CheckMapWall(collisionMapInfo);
	// 6.接地状態の切り替え
	//CheckMapLanding(collisionMapInfo);

	// 7.旋回制御
	//AnimateTurn();

	// アフィン変換行列
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//worldTransform_.TransferMatrix(); // 敵の座標の計算
    


	//worldTransform_.translation_ += velocity_;


	//walkTimer_ += 5.0f / 60.0f;

	//worldTransform_.rotation_.x = sin(walkTimer_);
	




	// 時間のカウンター
	walkTimer_ += 5.0f / 60.0f; // フレームごとの時間増分

	// 上下に揺れるように移動
	float amplitude = 20.0f; // 上下移動の幅（単位:座標）
	float speed = 0.1f;      // 速さ

	// Y方向にsinで移動
	worldTransform_.translation_.y = sin(walkTimer_ * speed) * amplitude;

	#pragma region 敵の移動

	// X,Y をいじるので一旦変数に出す
	Vector3& pos = worldTransform_.translation_;
	Vector3& rot = worldTransform_.rotation_;

	// 状態ごとに処理を分ける
	switch (state_) 
	{

	// ───────────────────────────────
	// ① 上下に揺れる状態
	// ───────────────────────────────
	case EnemyState::IdleMove:
		pos.y = sin(walkTimer_ * 0.1f) * 10.0f;
		rot.x = sin(walkTimer_);

		// ランダム行動タイマー
		actionTimer_ += 1.0f / 60.0f;

		// ランダム時間経過したら左移動へ
		if (actionTimer_ >= nextActionTime_)
		{
			state_ = EnemyState::MoveLeft;
			actionTimer_ = 0;
			// 次の発生時間を再設定
			nextActionTime_ = (rand() % 300) / 60.0f + 2.0f;
		}

		break;

	// ───────────────────────────────
	// ② 左に移動
	// ───────────────────────────────
	case EnemyState::MoveLeft:
		pos.x -= 0.2f; // 左へ移動速度

		if (pos.x <= startX_ - 15.0f)
		{ // 100だけ左に行ったら右へ戻る
			state_ = EnemyState::MoveRight;
		}
		break;

	// ───────────────────────────────
	// ③ 右に戻る
	// ───────────────────────────────
	case EnemyState::MoveRight:
		pos.x += 0.2f;

		if (pos.x >= startX_)
		{ // 初期位置まで戻ったら Idle へ
			pos.x = startX_;
			state_ = EnemyState::IdleMove;
		}
		break;
	}



	#pragma endregion

	if (enemyHp < 0)
	{
		isenemyDead_ = true;
	}
	





	// プレイヤーの座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); 
}

void Enemy::Draw() 
{
	if (isenemyDead_)
	{
		return;
	}
	model_->Draw(worldTransform_, *camera_); 
}




KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}


// 衝突応答
void Enemy::OnCollition(const Player* player)
{
	(void)player;
}

#pragma region 自キャラの弾と敵

AABB2 Enemy::GetAABB2()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Enemy::OnCollition2(const PlayerBullet* playerBullet)
{
	(void)playerBullet;
	enemyHp -= 100;
}


#pragma endregion








/*
// 2.マップ衝突チェック
void Enemy::CheckMapCollision(CollisionMapInfo& info) {
	CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

// マップ衝突チェック　上
void Enemy::CheckMapCollisionUP(CollisionMapInfo& info) {
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
void Enemy::CheckMapCollisionDown(CollisionMapInfo& info) {
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
void Enemy::CheckMapCollisionRight(CollisionMapInfo& info) {
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
void Enemy::CheckMapCollisionLeft(CollisionMapInfo& info) {
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
void Enemy::CheckMapMove(const CollisionMapInfo& info) 
{
	// 移動
	worldTransform_.translation_ += info.move;
}


// 4.天井に接触している場合の処理
void Enemy::CheckMapCeiling(const CollisionMapInfo& info) 
{
	if (info.ceiling)
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}


// 5.壁に接触している場合の処理
void Enemy::CheckMapWall(CollisionMapInfo& info)
{
	// 壁接触による減速
	if (info.hitwall)
	{
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}


// 6.接地状態の切り替え処理
void Enemy::CheckMapLanding(const CollisionMapInfo& info) 
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
void Enemy::AnimateTurn()
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


KamataEngine::Vector3 Enemy::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
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
*/