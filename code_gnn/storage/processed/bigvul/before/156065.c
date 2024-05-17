   void GetTimesStandardThrottlesAddedForURL(const GURL& url, int* count) {
    DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
    *count = times_stardard_throttles_added_for_url_[url];
  }
