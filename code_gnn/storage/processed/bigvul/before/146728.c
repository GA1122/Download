void Document::DidChangeVisibilityState() {
  DispatchEvent(Event::CreateBubble(EventTypeNames::visibilitychange));
  DispatchEvent(Event::CreateBubble(EventTypeNames::webkitvisibilitychange));

  if (GetPageVisibilityState() == kPageVisibilityStateVisible)
    Timeline().SetAllCompositorPending();

  if (hidden() && canvas_font_cache_)
    canvas_font_cache_->PruneAll();
}
