void SystemKeyEventListener::OnVolumeUp() {
  AudioHandler* audio_handler = GetAudioHandler();
  if (!audio_handler)
    return;

  if (audio_handler->IsMuted()) {
    audio_handler->SetMuted(false);
    if (audio_handler->GetVolumePercent() <= 0.1)   
      audio_handler->SetVolumePercent(kVolumePercentOnVolumeUpWhileMuted);
  } else {
    audio_handler->AdjustVolumeByPercent(kStepPercentage);
  }

  SendAccessibilityVolumeNotification(
      audio_handler->GetVolumePercent(),
      audio_handler->IsMuted());

  ShowVolumeBubble();
}
