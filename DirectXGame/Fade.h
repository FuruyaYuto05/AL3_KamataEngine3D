#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Fade {
public:
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

	enum class Type {
		kFade,    // 今まで通りのフワッとしたフェード
		kShutter, // 左右からバシッと閉じるシャッター
	};

	void Initialize();
	void Update();
	void Draw();

	void Start(Status status, float duration, Type type = Type::kFade);
	;

	void Stop();
	bool IsFinished() const;

private:
	Sprite* sprite_ = nullptr;

	// ★追加：シャッター用のスプライト（左右）
	Sprite* spriteLeft_ = nullptr;
	Sprite* spriteRight_ = nullptr;

	Status status_ = Status::None;
	Type type_ = Type::kFade;

	float duration_ = 0.0f;
	float counter_ = 0.0f;
};
