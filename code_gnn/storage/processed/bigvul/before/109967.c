void AudioOutputDevice::PauseOnIOThread(bool flush) {
  DCHECK(message_loop()->BelongsToCurrentThread());
  if (stream_id_ && is_started_) {
    ipc_->PauseStream(stream_id_);
    if (flush)
      ipc_->FlushStream(stream_id_);
  } else {
    play_on_start_ = false;
  }
}
