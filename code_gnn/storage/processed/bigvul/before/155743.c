void Compositor::SetExternalBeginFrameClient(ExternalBeginFrameClient* client) {
  DCHECK(external_begin_frames_enabled_);
  external_begin_frame_client_ = client;
  if (needs_external_begin_frames_ && external_begin_frame_client_)
    external_begin_frame_client_->OnNeedsExternalBeginFrames(true);
}
