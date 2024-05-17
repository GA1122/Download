  TestURLFetcherDelegate(
      scoped_refptr<net::URLRequestContextGetter> context_getter,
      const GURL& url,
      net::URLRequestStatus expected_request_status)
      : expected_request_status_(expected_request_status),
        fetcher_(net::URLFetcher::Create(url,
                                         net::URLFetcher::GET,
                                         this,
                                         TRAFFIC_ANNOTATION_FOR_TESTS)) {
    fetcher_->SetRequestContext(context_getter.get());
    fetcher_->Start();
  }
