  void StartObservingView(RenderWidgetHostViewAura* view) {
    if (view_)
      view_->set_paint_observer(NULL);

    loading_complete_ = false;
    received_paint_update_ = false;
    compositor_updated_ = false;
    view_ = view;
    if (view_)
      view_->set_paint_observer(this);

    if (window_.get() && window_->parent())
      window_->parent()->StackChildAtTop(window_.get());
  }
