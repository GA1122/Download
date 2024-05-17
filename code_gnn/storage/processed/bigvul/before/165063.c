CanvasResourceDispatcher* OffscreenCanvas::GetOrCreateResourceDispatcher() {
  DCHECK(HasPlaceholderCanvas());
  if (!frame_dispatcher_) {
    frame_dispatcher_ = std::make_unique<CanvasResourceDispatcher>(
        this, client_id_, sink_id_, placeholder_canvas_id_, size_);
  }
  return frame_dispatcher_.get();
}
