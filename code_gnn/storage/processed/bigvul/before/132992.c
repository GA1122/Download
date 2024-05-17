void RenderWidgetHostViewAura::OnLostResources() {
  current_surface_ = NULL;
  UpdateExternalTexture();

  idle_frame_subscriber_textures_.clear();
  yuv_readback_pipeline_.reset();

  RunOnCommitCallbacks();
  host_->ScheduleComposite();
}
