void PepperPlatformAudioInput::StopCapture() {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  io_message_loop_proxy_->PostTask(
      FROM_HERE,
      base::Bind(&PepperPlatformAudioInput::StopCaptureOnIOThread, this));
}
