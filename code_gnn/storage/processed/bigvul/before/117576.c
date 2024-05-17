void SettingLevelBubble::HideBubble() {
  hide_timer_.Stop();
  if (view_) {
    view_->GetWidget()->Close();
    view_ = NULL;
  }
}
