  virtual ~OverscrollNavigationOverlay() {
    if (view_)
      view_->set_paint_observer(NULL);
  }
