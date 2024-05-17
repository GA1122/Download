void Document::DidChangeVisibilityState() {
  DispatchEvent(*Event::CreateBubble(event_type_names::kVisibilitychange));
  DispatchEvent(
      *Event::CreateBubble(event_type_names::kWebkitvisibilitychange));

  if (IsPageVisible())
    Timeline().SetAllCompositorPending();

  if (hidden() && canvas_font_cache_)
    canvas_font_cache_->PruneAll();

  InteractiveDetector* interactive_detector = InteractiveDetector::From(*this);
  if (interactive_detector) {
    interactive_detector->OnPageHiddenChanged(hidden());
  }
}
