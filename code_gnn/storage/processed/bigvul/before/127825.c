void PepperPlatformAudioInput::NotifyStreamCreationFailed() {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  if (client_)
    client_->StreamCreationFailed();
}
