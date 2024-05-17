gfx::NativeWindow LoginDisplayHostWebUI::GetNativeWindow() const {
  return login_window_ ? login_window_->GetNativeWindow() : nullptr;
}
