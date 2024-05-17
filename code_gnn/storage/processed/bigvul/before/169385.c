void RenderFrameSubmissionObserver::WaitForScrollOffset(
    const gfx::Vector2dF& expected_offset) {
  while (render_frame_metadata_provider_->LastRenderFrameMetadata()
             .root_scroll_offset != expected_offset) {
    WaitForMetadataChange();
  }
}
