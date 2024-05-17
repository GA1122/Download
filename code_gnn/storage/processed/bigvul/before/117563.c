void SystemKeyEventListener::OnVolumeMute() {
  AudioHandler* audio_handler = GetAudioHandler();
  if (!audio_handler)
    return;

  audio_handler->SetMuted(true);

  SendAccessibilityVolumeNotification(
      audio_handler->GetVolumePercent(),
      audio_handler->IsMuted());

  ShowVolumeBubble();
}
