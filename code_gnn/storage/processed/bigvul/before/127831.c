void PepperPlatformAudioInput::ShutDown() {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  if (!client_)
    return;

  client_ = NULL;
  io_message_loop_proxy_->PostTask(
      FROM_HERE,
      base::Bind(&PepperPlatformAudioInput::ShutDownOnIOThread, this));
}
