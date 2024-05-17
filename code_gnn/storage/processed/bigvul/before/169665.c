  void StartMonitoringRequestsFor(const GURL& url) {
    request_url_ = url;
    saw_request_url_ = false;
  }
