    RenderFrameHostImpl::AccessibilityGetAcceleratedWidget() {
  if (frame_tree_node()->parent() ||
      frame_tree_node()->current_frame_host() != this) {
    return gfx::kNullAcceleratedWidget;
  }

  RenderWidgetHostViewBase* view = static_cast<RenderWidgetHostViewBase*>(
      render_view_host_->GetWidget()->GetView());
  if (view)
    return view->AccessibilityGetAcceleratedWidget();
  return gfx::kNullAcceleratedWidget;
}
