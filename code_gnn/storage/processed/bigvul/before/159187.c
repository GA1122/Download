DownloadItemImpl::RequestInfo::RequestInfo(const GURL& url)
    : url_chain(std::vector<GURL>(1, url)), start_time(base::Time::Now()) {}
