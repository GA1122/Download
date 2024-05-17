void StatusBubbleGtk::ExpandURL() {
  start_width_ = label_->allocation.width;
  expand_animation_.reset(new ui::SlideAnimation(this));
  expand_animation_->SetTweenType(ui::Tween::LINEAR);
  expand_animation_->Show();

  SetStatusTextToURL();
}
