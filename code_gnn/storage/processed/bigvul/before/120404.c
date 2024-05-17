  virtual ~ScopedGestureRecognizerSetter() {
    ui::SetGestureRecognizerForTesting(original_gr_);
  }
