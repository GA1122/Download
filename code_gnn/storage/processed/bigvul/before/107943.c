gfx::Size InfoBar::GetPreferredSize() {
  int height = static_cast<int>(target_height_ * animation_->GetCurrentValue());
  return gfx::Size(0, height);
}
