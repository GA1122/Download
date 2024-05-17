void WebSocketExperimentTask::OnURLFetchComplete(
    const URLFetcher* source,
    const GURL& url,
    const URLRequestStatus& status,
    int response_code,
    const ResponseCookies& cookies,
    const std::string& data) {
  result_.url_fetch = base::TimeTicks::Now() - url_fetch_start_time_;
  RevokeTimeoutTimer();
  int result = net::ERR_FAILED;
  if (next_state_ != STATE_URL_FETCH_COMPLETE) {
    DLOG(INFO) << "unexpected state=" << next_state_;
    result = net::ERR_UNEXPECTED;
  } else if (response_code == 200 || response_code == 304) {
    result = net::OK;
  }
  DoLoop(result);
}
