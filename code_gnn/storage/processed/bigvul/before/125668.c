void RenderViewHostImpl::OnRenderAutoResized(const gfx::Size& new_size) {
  delegate_->ResizeDueToAutoResize(new_size);
}
