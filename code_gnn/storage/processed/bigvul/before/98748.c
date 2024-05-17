void WebPluginDelegatePepper::UpdateGeometry(
    const gfx::Rect& window_rect,
    const gfx::Rect& clip_rect) {
  if (window_rect == window_rect_ && clip_rect == clip_rect_)
    return;

  clip_rect_ = clip_rect;
  cutout_rects_.clear();

  if (window_rect_ == window_rect)
    return;
  window_rect_ = window_rect;

  SkBitmap new_committed;
  new_committed.setConfig(SkBitmap::kARGB_8888_Config,
                          window_rect_.width(), window_rect_.height());
  new_committed.allocPixels();
  committed_bitmap_ = new_committed;

  if (nested_delegate_)
    nested_delegate_->UpdateGeometry(window_rect, clip_rect);

#if defined(ENABLE_GPU)
#if defined(OS_MACOSX)
  if (command_buffer_) {
    command_buffer_->SetWindowSize(window_rect_.size());
  }
#endif   
#endif   

  if (!instance())
    return;

  ForwardSetWindow();
}
