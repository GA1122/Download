  virtual void AnimationProgressed(const ui::Animation* animation) {
    TabAnimation::AnimationProgressed(animation);

    int x = animation_.CurrentValueBetween(start_bounds_.x(),
                                           target_bounds_.x());
    int width = animation_.CurrentValueBetween(start_bounds_.width(),
                                               target_bounds_.width());
    gfx::Rect tab_bounds(x, start_bounds_.y(), width,
                         start_bounds_.height());
    tabstrip_->SetTabBounds(tab_, tab_bounds);
  }
