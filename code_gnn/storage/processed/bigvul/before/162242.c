void GamepadProvider::SendPauseHint(bool paused) {
  DCHECK(polling_thread_->task_runner()->BelongsToCurrentThread());
  for (const auto& it : data_fetchers_) {
    it->PauseHint(paused);
  }
}
