  void SetRenderFrameMetadata(cc::RenderFrameMetadata metadata) {
    host()->render_frame_metadata_provider()->SetLastRenderFrameMetadataForTest(
        metadata);
  }
