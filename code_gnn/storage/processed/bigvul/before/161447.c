  void TrackOriginOnIOThread(const url::Origin& origin) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    if (origins_.find(origin) != origins_.end())
      return;
    origins_.insert(origin);
  }
