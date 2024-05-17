  void PartialMagnificationController::OnWidgetDestroying(views::Widget* widget) {
  DCHECK_EQ(widget, zoom_widget_);
//   DCHECK_EQ(widget, host_widget_);
    RemoveZoomWidgetObservers();
  zoom_widget_ = NULL;
//   host_widget_ = nullptr;
  }