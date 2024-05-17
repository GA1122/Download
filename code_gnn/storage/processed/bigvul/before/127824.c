void PepperPlatformAudioInput::InitializeOnIOThread(int session_id) {
  DCHECK(io_message_loop_proxy_->BelongsToCurrentThread());

  if (!ipc_)
    return;

  create_stream_sent_ = true;
  ipc_->CreateStream(this, session_id, params_, false, 1);
}
