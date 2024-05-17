void RenderWidgetHostImpl::DelayedAutoResized() {
  gfx::Size new_size = new_auto_size_;
  new_auto_size_.SetSize(0, 0);
  if (!auto_resize_enabled_)
    return;

  if (view_) {
    viz::ScopedSurfaceIdAllocator scoped_allocator =
        view_->ResizeDueToAutoResize(new_size,
                                     last_auto_resize_request_number_);

    if (delegate_) {
      delegate_->ResizeDueToAutoResize(this, new_size,
                                       last_auto_resize_request_number_);
    }
  }
}
