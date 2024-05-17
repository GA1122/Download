void RenderWidgetHostImpl::RendererExited(base::TerminationStatus status,
                                          int exit_code) {
  renderer_initialized_ = false;

  waiting_for_screen_rects_ack_ = false;

  mouse_move_pending_ = false;
  next_mouse_move_.reset();
  mouse_wheel_pending_ = false;
  coalesced_mouse_wheel_events_.clear();

  select_range_pending_ = false;
  next_selection_range_.reset();

  touch_event_queue_->Reset();

  gesture_event_filter_->Reset();

  if (overscroll_controller_.get())
    overscroll_controller_->Reset();

  key_queue_.clear();
  suppress_next_char_events_ = false;

  ResetSizeAndRepaintPendingFlags();
  current_size_.SetSize(0, 0);
  is_hidden_ = false;
  is_accelerated_compositing_active_ = false;

  in_flight_event_count_ = 0;

  if (view_) {
    GpuSurfaceTracker::Get()->SetSurfaceHandle(surface_id_,
                                               gfx::GLSurfaceHandle());
    view_->RenderViewGone(status, exit_code);
    view_ = NULL;   
  }

  BackingStoreManager::RemoveBackingStore(this);
}
