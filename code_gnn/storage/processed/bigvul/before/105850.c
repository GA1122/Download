void UrlmonUrlRequestManager::StopAll() {
  DVLOG(1) << __FUNCTION__;
  if (stopping_)
    return;

  stopping_ = true;

  DVLOG(1) << __FUNCTION__ << " stopping " << request_map_.size()
           << " requests";

  for (RequestMap::iterator it = request_map_.begin();
       it != request_map_.end(); ++it) {
    DCHECK(it->second != NULL);
    it->second->Stop();
  }

  request_map_.clear();
}
