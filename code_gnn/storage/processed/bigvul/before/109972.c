void AudioOutputDevice::SetVolumeOnIOThread(double volume) {
  DCHECK(message_loop()->BelongsToCurrentThread());
  if (stream_id_)
    ipc_->SetVolume(stream_id_, volume);
}
