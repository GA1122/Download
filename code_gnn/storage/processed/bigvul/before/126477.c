  virtual void AnimationEnded(const ui::Animation* animation) {
    tabstrip_->RemoveTabAt(index_);
    TabStripGtk::TabAnimation::AnimationEnded(animation);
  }
