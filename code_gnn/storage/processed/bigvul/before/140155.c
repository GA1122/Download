void CaptivePortalDetector::GetCaptivePortalResultFromResponse(
    const net::URLFetcher* url_fetcher,
    Results* results) const {
  DCHECK(results);
  DCHECK(!url_fetcher->GetStatus().is_io_pending());

  results->result = captive_portal::RESULT_NO_RESPONSE;
  results->response_code = url_fetcher->GetResponseCode();
  results->retry_after_delta = base::TimeDelta();
  results->landing_url = url_fetcher->GetURL();

  VLOG(1) << "Getting captive portal result"
          << " response code: " << results->response_code
          << " landing_url: " << results->landing_url;

  if (url_fetcher->GetStatus().status() != net::URLRequestStatus::SUCCESS)
    return;

  if (results->response_code == 503) {
    net::HttpResponseHeaders* headers = url_fetcher->GetResponseHeaders();
    std::string retry_after_string;

    if (!headers->EnumerateHeader(NULL, "Retry-After", &retry_after_string))
      return;

    base::TimeDelta retry_after_delta;
    if (net::HttpUtil::ParseRetryAfterHeader(retry_after_string,
                                             GetCurrentTime(),
                                             &retry_after_delta)) {
      results->retry_after_delta = retry_after_delta;
    }

    return;
  }

  if (results->response_code == 511) {
    results->result = captive_portal::RESULT_BEHIND_CAPTIVE_PORTAL;
    return;
  }

  if (results->response_code >= 400 || results->response_code < 200)
    return;

  if (results->response_code == 204) {
    results->result = captive_portal::RESULT_INTERNET_CONNECTED;
    return;
  }

  results->result = captive_portal::RESULT_BEHIND_CAPTIVE_PORTAL;
}
