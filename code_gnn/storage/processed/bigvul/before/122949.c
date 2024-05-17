 void RenderWidgetHostImpl::DelayedAutoResized() {
  gfx::Size new_size = new_auto_size_;
  new_auto_size_.SetSize(0, 0);
  if (!should_auto_resize_)
    return;

  OnRenderAutoResized(new_size);
}
