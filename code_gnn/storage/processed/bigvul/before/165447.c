void Document::DidChangeVisibilityState() {
  DispatchEvent(*Event::CreateBubble(event_type_names::kVisibilitychange));
  DispatchEvent(
      *Event::CreateBubble(event_type_names::kWebkitvisibilitychange));

  if (GetPageVisibilityState() == mojom::PageVisibilityState::kVisible)
    Timeline().SetAllCompositorPending();

  if (hidden() && canvas_font_cache_)
    canvas_font_cache_->PruneAll();

  InteractiveDetector* interactive_detector = InteractiveDetector::From(*this);
  if (interactive_detector) {
    interactive_detector->OnPageVisibilityChanged(GetPageVisibilityState());
  }
}
