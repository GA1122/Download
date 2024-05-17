  virtual void AnimationProgressed(const ui::Animation* animation) {
    tabstrip_->AnimationLayout(end_unselected_width_);
  }
