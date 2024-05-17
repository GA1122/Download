bool DownloadRequestLimiter::TabDownloadState::is_showing_prompt() const {
  return factory_.HasWeakPtrs();
}
