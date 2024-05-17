ResizeObserverController& Document::EnsureResizeObserverController() {
  if (!resize_observer_controller_)
    resize_observer_controller_ = new ResizeObserverController();
  return *resize_observer_controller_;
}
