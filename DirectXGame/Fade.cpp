#include "Fade.h"
#include <algorithm>

void Fade::Initialize() {
	sprite_ = Sprite::Create(0, Vector2{});
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1.f));

	// シャッター用（左半分）
	spriteLeft_ = Sprite::Create(0, Vector2{});
	spriteLeft_->SetSize(Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight)); // 幅は半分
	spriteLeft_->SetColor(Vector4(0, 0, 0, 1.f));                                      // 黒（後で画像に変えればヒビ割れにできます）

	// シャッター用（右半分）
	spriteRight_ = Sprite::Create(0, Vector2{});
	spriteRight_->SetSize(Vector2(WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight)); // 幅は半分
	spriteRight_->SetColor(Vector4(0, 0, 0, 1.f));
}

void Fade::Update() {
	switch (status_) {
	case Status::None:
		break;

	case Status::FadeIn:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		if (type_ == Type::kFade) {
			// 今までのフェード（透明度）
			sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));
		} else {
			// シャッター（座標を外側に逃がしていく）
			float progress = counter_ / duration_; // 0.0 -> 1.0
			float moveX = (WinApp::kWindowWidth / 2.0f) * progress;
			spriteLeft_->SetPosition({-moveX, 0});
			spriteRight_->SetPosition({WinApp::kWindowWidth / 2.0f + moveX, 0});
		}
		break;

	case Status::FadeOut:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		if (type_ == Type::kFade) {
			// 今までのフェード（透明度）
			sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		} else {
			// シャッター（外側から中央へ寄せていく）
			float progress = counter_ / duration_;      // 0.0 -> 1.0
			float startX = WinApp::kWindowWidth / 2.0f; // 画面端まで飛ばす距離
			float moveX = startX * (1.0f - progress);
			spriteLeft_->SetPosition({-moveX, 0});
			spriteRight_->SetPosition({WinApp::kWindowWidth / 2.0f + moveX, 0});
		}
		break;
	}
}

void Fade::Draw() {
	if (status_ == Status::None) {
		return;
	}

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (type_ == Type::kFade) {
		sprite_->Draw();
	} else {
		spriteLeft_->Draw();
		spriteRight_->Draw();
	}

	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration,Type type) {

	status_ = status;
	duration_ = duration;
	type_ = type;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {

	// フェード状態による分岐
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:
		/*
		        if (counter_ >= duration_) {
		            return true;
		        }
		        else {
		            return false;
		        }
		*/
		return (counter_ >= duration_) ? true : false;
	}

	return true;
}
