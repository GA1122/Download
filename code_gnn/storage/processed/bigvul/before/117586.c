void SettingLevelBubble::UpdateWithoutShowingBubble(double percent,
                                                    bool enabled) {
  UpdateTargetPercent(percent);
  if (view_)
    view_->SetEnabled(enabled);
}
