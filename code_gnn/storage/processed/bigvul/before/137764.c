void HeadlessWebContentsImpl::OnDisplayDidFinishFrame(
    const viz::BeginFrameAck& ack) {
  TRACE_EVENT2("headless", "HeadlessWebContentsImpl::OnDisplayDidFinishFrame",
               "source_id", ack.source_id, "sequence_number",
               ack.sequence_number);

  auto it = pending_frames_.begin();
  while (it != pending_frames_.end()) {
    if (begin_frame_source_id_ == ack.source_id &&
        (*it)->sequence_number <= ack.sequence_number) {
      (*it)->has_damage = ack.has_damage;
      (*it)->display_did_finish_frame = true;
      if ((*it)->MaybeRunCallback()) {
        it = pending_frames_.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }
}
