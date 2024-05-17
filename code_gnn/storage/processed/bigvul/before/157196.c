bool WebMediaPlayerImpl::DoesOverlaySupportMetadata() const {
  return pipeline_metadata_.video_decoder_config.video_rotation() ==
         VIDEO_ROTATION_0;
}
