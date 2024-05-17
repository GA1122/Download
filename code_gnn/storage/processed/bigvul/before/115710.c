void ScreenRecorder::DoSendVideoPacket(VideoPacket* packet) {
  DCHECK(network_loop_->BelongsToCurrentThread());

  bool last = (packet->flags() & VideoPacket::LAST_PARTITION) != 0;

  if (network_stopped_ || connections_.empty()) {
    delete packet;
    return;
  }

  for (ConnectionToClientList::const_iterator i = connections_.begin();
       i < connections_.end(); ++i) {
    base::Closure done_task;

    if (last && i == connections_.begin()) {
      done_task = base::Bind(&ScreenRecorder::FrameSentCallback, this, packet);
    } else {
      done_task = base::Bind(&DeletePointer<VideoPacket>, packet);
    }

    (*i)->video_stub()->ProcessVideoPacket(packet, done_task);
  }
}
