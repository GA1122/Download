  void UntrackOriginOnIOThread(const url::Origin& origin) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    origins_.erase(origin);
  }
