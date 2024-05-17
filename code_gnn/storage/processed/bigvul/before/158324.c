void RenderWidgetHostImpl::OnRenderFrameMetadataChangedAfterActivation() {
  bool is_mobile_optimized =
      render_frame_metadata_provider_.LastRenderFrameMetadata()
          .is_mobile_optimized;
  input_router_->NotifySiteIsMobileOptimized(is_mobile_optimized);
  if (auto* touch_emulator = GetExistingTouchEmulator())
    touch_emulator->SetDoubleTapSupportForPageEnabled(!is_mobile_optimized);
}
