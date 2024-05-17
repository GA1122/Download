void DefaultAudioDestinationHandler::StartPlatformDestination() {
  if (platform_destination_->IsPlaying()) {
    return;
  }

  AudioWorklet* audio_worklet = Context()->audioWorklet();
  if (audio_worklet && audio_worklet->IsReady()) {
    platform_destination_->StartWithWorkletTaskRunner(
        audio_worklet->GetMessagingProxy()
            ->GetBackingWorkerThread()
            ->GetTaskRunner(TaskType::kInternalMedia));
  } else {
    platform_destination_->Start();
  }
}
