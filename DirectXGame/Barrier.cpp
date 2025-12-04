#include "Barrier.h"
#include<algorithm>
#include<numbers>

#include "cassert"
#define NOMINMAX
#include "MapChipField.h"

using namespace KamataEngine;
using namespace MathUtility;




void Barrier::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) 
{
	// NULLポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}


void Barrier::Update()
{
	// プレイヤーの座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Barrier::Draw() { model_->Draw(worldTransform_, *camera_); }


