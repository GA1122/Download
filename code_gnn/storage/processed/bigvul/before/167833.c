DownloadRequestLimiter::TabDownloadState::~TabDownloadState() {
  DCHECK(callbacks_.empty());

  DCHECK(!factory_.HasWeakPtrs());
}
