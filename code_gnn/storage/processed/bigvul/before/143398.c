void Job::StartGetProxyForURL(const GURL& url,
                              ProxyInfo* results,
                              const CompletionCallback& callback) {
  CheckIsOnOriginThread();

  url_ = url;
  user_results_ = results;

  Start(GET_PROXY_FOR_URL, false  , callback);
}
