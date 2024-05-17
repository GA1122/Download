void PepperPlatformAudioInput::StartCapture() {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  io_message_loop_proxy_->PostTask(
      FROM_HERE,
      base::Bind(&PepperPlatformAudioInput::StartCaptureOnIOThread, this));
}
