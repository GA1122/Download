AppCacheUpdateJob::URLFetcher::URLFetcher(const GURL& url,
                                          FetchType fetch_type,
                                          AppCacheUpdateJob* job)
    : url_(url),
      job_(job),
      fetch_type_(fetch_type),
      retry_503_attempts_(0),
      buffer_(new net::IOBuffer(kBufferSize)),
      request_(job->service_->request_context()
                   ->CreateRequest(url, net::DEFAULT_PRIORITY, this)),
      result_(UPDATE_OK),
      redirect_response_code_(-1) {}
