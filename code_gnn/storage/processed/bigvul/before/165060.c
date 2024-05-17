void OffscreenCanvas::DiscardResourceProvider() {
  CanvasResourceHost::DiscardResourceProvider();
  needs_matrix_clip_restore_ = true;
}
