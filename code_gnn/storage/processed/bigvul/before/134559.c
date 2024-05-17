  void StopObservingIfDone() {
    if ((need_paint_update_ && !received_paint_update_) ||
        (image_delegate_->has_image() && !loading_complete_)) {
      return;
    }

    if (window_slider_.get() && window_slider_->IsSlideInProgress())
      return;

    window_slider_.reset();
    window_.reset();
    image_delegate_ = NULL;
    if (view_) {
      view_->set_paint_observer(NULL);
      view_ = NULL;
    }
  }
