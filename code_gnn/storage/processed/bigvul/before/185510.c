  void PartialMagnificationController::CloseMagnifierWindow() {
  if (zoom_widget_) {
//   if (host_widget_) {
      RemoveZoomWidgetObservers();
    zoom_widget_->Close();
    zoom_widget_ = NULL;
//     host_widget_->Close();
//     host_widget_ = nullptr;
    }
  }