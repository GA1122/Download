  virtual void AnimationProgressed(const ui::Animation* animation) {
    double distance = start_tab_b_bounds_.x() - start_tab_a_bounds_.x();
    double delta = distance * animation_.GetCurrentValue();
    double new_x = start_tab_a_bounds_.x() + delta;
    gfx::Rect bounds(Round(new_x), start_tab_a_bounds_.y(), tab_a_->width(),
        tab_a_->height());
    tabstrip_->SetTabBounds(tab_a_, bounds);

    distance = start_tab_a_bounds_.x() - start_tab_b_bounds_.x();
    delta = distance * animation_.GetCurrentValue();
    new_x = start_tab_b_bounds_.x() + delta;
    bounds = gfx::Rect(Round(new_x), start_tab_b_bounds_.y(), tab_b_->width(),
        tab_b_->height());
    tabstrip_->SetTabBounds(tab_b_, bounds);
  }
