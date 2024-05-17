void SettingLevelBubble::ShowBubble(double percent, bool enabled) {
  hide_timer_.Stop();

  const double old_target_percent = target_percent_;
  UpdateTargetPercent(percent);
  SkBitmap* current_icon = increase_icon_;
  if (!enabled || target_percent_ == 0)
    current_icon = disabled_icon_;
  else if (old_target_percent >= 0 && target_percent_ < old_target_percent)
    current_icon = decrease_icon_;

  if (!view_) {
    view_ = CreateView();
    view_->Init(current_icon, percent, enabled);
  } else {
    SettingLevelBubbleDelegateView* delegate =
        static_cast<SettingLevelBubbleDelegateView*>
        (view_->GetWidget()->widget_delegate());
    delegate->ResetFade();
    view_->SetIcon(current_icon);
    view_->SetEnabled(enabled);
  }
  view_->GetWidget()->Show();
  hide_timer_.Start(FROM_HERE,
                    base::TimeDelta::FromMilliseconds(kBubbleShowTimeoutMs),
                    this, &SettingLevelBubble::OnHideTimeout);
}
