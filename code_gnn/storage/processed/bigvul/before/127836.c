void PepperPlatformAudioInput::StopCaptureOnIOThread() {
  DCHECK(io_message_loop_proxy_->BelongsToCurrentThread());

  if (ipc_ && create_stream_sent_) {
    ipc_->CloseStream();
  }
  ipc_.reset();
}
