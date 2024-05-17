void SynchronousCompositorImpl::UpdateNeedsBeginFrames() {
  RenderWidgetHostViewAndroid* rwhv = static_cast<RenderWidgetHostViewAndroid*>(
      contents_->GetRenderWidgetHostView());
  if (rwhv)
     rwhv->OnSetNeedsBeginFrames(is_active_ && renderer_needs_begin_frames_);
 }
