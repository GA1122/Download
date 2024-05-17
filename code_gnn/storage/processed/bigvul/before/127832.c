void PepperPlatformAudioInput::ShutDownOnIOThread() {
  DCHECK(io_message_loop_proxy_->BelongsToCurrentThread());

  StopCaptureOnIOThread();

  main_message_loop_proxy_->PostTask(
      FROM_HERE, base::Bind(&PepperPlatformAudioInput::CloseDevice, this));

  Release();   
}
