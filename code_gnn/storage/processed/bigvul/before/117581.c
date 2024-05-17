void SettingLevelBubble::OnWidgetClosing(views::Widget* widget) {
  if (view_ && view_->GetWidget() == widget) {
    view_->GetWidget()->RemoveObserver(this);
    view_ = NULL;
  }
  current_percent_ = target_percent_;
  target_time_ = TimeTicks();
  last_animation_update_time_ = TimeTicks();
  last_target_update_time_ = TimeTicks();
  hide_timer_.Stop();
  StopAnimation();
}
