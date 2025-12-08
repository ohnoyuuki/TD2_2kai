#pragma once
#include"KamataEngine.h"
#include"MyMath.h"

class MapChipField;


class Barrier 
{
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);

	void Update();

	void Draw();



	
	// デスフラグ
	bool barrierDead_ = false;
	// デスフラグのgetter
	bool BarrierDead() const { return barrierDead_; }
	// バリアの当たり判定サイズ
	static inline const float kWidth = 30.0f;
	static inline const float kHeight = 720.0f;


	// 体力表示
	int BarrierHp;


private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
