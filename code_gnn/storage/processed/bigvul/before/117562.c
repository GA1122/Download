void SystemKeyEventListener::OnVolumeDown() {
  AudioHandler* audio_handler = GetAudioHandler();
  if (!audio_handler)
    return;

  if (audio_handler->IsMuted())
    audio_handler->SetVolumePercent(0.0);
  else
    audio_handler->AdjustVolumeByPercent(-kStepPercentage);

  SendAccessibilityVolumeNotification(
      audio_handler->GetVolumePercent(),
      audio_handler->IsMuted());

  ShowVolumeBubble();
}
