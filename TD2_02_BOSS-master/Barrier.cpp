#include "Barrier.h"
#include<algorithm>
#include<numbers>
#include "cassert"
#define NOMINMAX
#include "MapChipField.h"

using namespace KamataEngine;
using namespace MathUtility;




void Barrier::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera,KamataEngine::Vector3& position) {
	// NULLポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	model_ = model;

	textureHandle_ = textureHandle;
	
	worldTransform_.translation_ = position;
	
	worldTransform_.Initialize();


	BarrierHp = 50000;

}


void Barrier::Update()
{
	// 座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();



	if (BarrierHp < 0) 
	{
		barrierDead_ = true;
	}
}

void Barrier::Draw() { model_->Draw(worldTransform_, *camera_); }


KamataEngine::Vector3 Barrier::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region バリアと敵の弾の衝突

AABB4 Barrier::GetAABB4() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB4 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// バリアと敵の弾の衝突
void Barrier::OnCollition4(const EnemyBullet* enemyBullet) 
{
	(void)enemyBullet; 
	BarrierHp -= 1;
	hp_ -= 1;
	if (hp_ <= 0)
	{
		hp_ = 0;
		barrierDead_ = true;
	}
}

#pragma endregion
