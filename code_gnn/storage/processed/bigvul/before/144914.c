gfx::Size RenderWidgetHostViewAura::GetRequestedRendererSize() const {
  return delegated_frame_host_
             ? delegated_frame_host_->GetRequestedRendererSize()
             : RenderWidgetHostViewBase::GetRequestedRendererSize();
}
