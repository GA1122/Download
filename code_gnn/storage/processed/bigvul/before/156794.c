void Document::DidChangeVisibilityState() {
  DispatchEvent(*Event::CreateBubble(EventTypeNames::visibilitychange));
  DispatchEvent(*Event::CreateBubble(EventTypeNames::webkitvisibilitychange));

  if (GetPageVisibilityState() == mojom::PageVisibilityState::kVisible)
    Timeline().SetAllCompositorPending();

  if (hidden() && canvas_font_cache_)
    canvas_font_cache_->PruneAll();

  InteractiveDetector* interactive_detector = InteractiveDetector::From(*this);
  if (interactive_detector) {
    interactive_detector->OnPageVisibilityChanged(GetPageVisibilityState());
  }
}
