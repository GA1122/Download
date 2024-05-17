float RenderFrameHostImpl::AccessibilityGetDeviceScaleFactor() const {
  RenderWidgetHostView* view = render_view_host_->GetWidget()->GetView();
  if (view)
    return GetScaleFactorForView(view);
  return 1.0f;
}
