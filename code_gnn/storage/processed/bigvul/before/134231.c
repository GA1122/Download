void BrowserRootView::DispatchGestureEvent(ui::GestureEvent* event) {
  if (event->type() == ui::ET_GESTURE_TAP &&
      event->location().y() <= 0 &&
      event->location().x() <= browser_view_->GetBounds().width()) {
    TouchUMA::RecordGestureAction(TouchUMA::GESTURE_ROOTVIEWTOP_TAP);
  }

  RootView::DispatchGestureEvent(event);
}
