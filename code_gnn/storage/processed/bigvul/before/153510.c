TabStrip::~TabStrip() {
  StopAnimating(false);

  drag_context_->DestroyDragController();

  RemoveMessageLoopObserver();

  RemoveAllChildViews(true);
}
