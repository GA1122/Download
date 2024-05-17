void AudioInputRendererHost::SendErrorMessage(int stream_id) {
  Send(new AudioInputMsg_NotifyStreamStateChanged(
      stream_id, media::AudioInputIPCDelegate::kError));
}
