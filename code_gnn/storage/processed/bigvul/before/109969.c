void AudioOutputDevice::PlayOnIOThread() {
  DCHECK(message_loop()->BelongsToCurrentThread());
  if (stream_id_ && is_started_)
    ipc_->PlayStream(stream_id_);
  else
    play_on_start_ = true;
}
