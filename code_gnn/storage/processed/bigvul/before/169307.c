bool WebContentsAddedObserver::RenderViewCreatedCalled() {
  if (child_observer_) {
    return child_observer_->render_view_created_called_ &&
           child_observer_->main_frame_created_called_;
  }
  return false;
}
