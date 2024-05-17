  explicit AnimationWaiter(views::View* host)
      : animator_(host->layer()->GetAnimator()) {
    animator_->AddObserver(this);
  }
