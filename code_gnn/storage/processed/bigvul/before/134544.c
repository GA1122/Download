  void SetOverlayWindow(scoped_ptr<aura::Window> window,
                        ImageWindowDelegate* delegate) {
    window_ = window.Pass();
    if (window_.get() && window_->parent())
      window_->parent()->StackChildAtTop(window_.get());
    image_delegate_ = delegate;

    if (window_.get() && delegate->has_image()) {
      window_slider_.reset(new WindowSlider(this,
                                            window_->parent(),
                                            window_.get()));
      slide_direction_ = SLIDE_UNKNOWN;
    } else {
      window_slider_.reset();
    }
  }
