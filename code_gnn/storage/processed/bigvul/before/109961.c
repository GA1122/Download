void AudioOutputDevice::CreateStreamOnIOThread(const AudioParameters& params,
                                               int input_channels) {
  DCHECK(message_loop()->BelongsToCurrentThread());
  DCHECK_EQ(0, stream_id_);
  if (stream_id_)
    return;

  stream_id_ = ipc_->AddDelegate(this);
  ipc_->CreateStream(stream_id_, params, input_channels);
}
