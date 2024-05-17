void PepperPlatformAudioInput::StartCaptureOnIOThread() {
  DCHECK(io_message_loop_proxy_->BelongsToCurrentThread());

  if (ipc_)
    ipc_->RecordStream();
}
