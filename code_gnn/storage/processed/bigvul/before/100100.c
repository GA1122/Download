void BrowserActionsContainer::AnimationProgressed(const Animation* animation) {
  DCHECK(animation == resize_animation_.get());

  double e = resize_animation_->GetCurrentValue();
  int difference = container_size_.width() - animation_target_size_;

  resize_amount_ = static_cast<int>(e * difference);

  OnBrowserActionVisibilityChanged();
}
