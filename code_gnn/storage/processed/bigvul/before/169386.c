void RenderFrameSubmissionObserver::WaitForScrollOffsetAtTop(
    bool expected_scroll_offset_at_top) {
  while (render_frame_metadata_provider_->LastRenderFrameMetadata()
             .is_scroll_offset_at_top != expected_scroll_offset_at_top) {
    WaitForMetadataChange();
  }
}
