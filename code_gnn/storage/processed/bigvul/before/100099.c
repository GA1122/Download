void BrowserActionsContainer::AnimationEnded(const Animation* animation) {
  container_size_.set_width(animation_target_size_);
  animation_target_size_ = 0;
  resize_amount_ = 0;
  OnBrowserActionVisibilityChanged();
  suppress_chevron_ = false;

  profile_->GetPrefs()->SetInteger(prefs::kBrowserActionContainerWidth,
                                   container_size_.width());
}
