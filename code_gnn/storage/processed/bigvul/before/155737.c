void Compositor::OnDisplayDidFinishFrame(const viz::BeginFrameAck& ack) {
  DCHECK(external_begin_frames_enabled_);
  if (external_begin_frame_client_)
    external_begin_frame_client_->OnDisplayDidFinishFrame(ack);
}
