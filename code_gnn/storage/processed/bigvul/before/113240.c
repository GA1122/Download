ui::TouchStatus ShelfLayoutManager::AutoHideEventFilter::PreHandleTouchEvent(
    aura::Window* target,
    aura::TouchEvent* event) {
  return ui::TOUCH_STATUS_UNKNOWN;   
}
