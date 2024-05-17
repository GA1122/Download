void OfflineAudioDestinationHandler::RestartRendering() {
  DCHECK(IsMainThread());

  PrepareTaskRunnerForRendering();
};
