void WebContentsImpl::ResizeDueToAutoResize(
    RenderWidgetHostImpl* render_widget_host,
    const gfx::Size& new_size,
    uint64_t sequence_number) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  auto_resize_size_ = new_size;

  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    if (node->current_frame_host()->is_local_root()) {
      RenderWidgetHostImpl* host =
          node->current_frame_host()->GetRenderWidgetHost();
      if (host != render_widget_host)
        host->WasResized();
    }
  }

  if (delegate_)
    delegate_->ResizeDueToAutoResize(this, new_size);
}
