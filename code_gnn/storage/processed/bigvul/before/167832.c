DownloadRequestLimiter::~DownloadRequestLimiter() {
  DCHECK(state_map_.empty());
}
