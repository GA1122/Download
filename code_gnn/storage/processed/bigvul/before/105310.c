void AutofillDownloadManager::OnURLFetchComplete(
    const URLFetcher* source,
    const GURL& url,
    const net::URLRequestStatus& status,
    int response_code,
    const net::ResponseCookies& cookies,
    const std::string& data) {
  std::map<URLFetcher *, FormRequestData>::iterator it =
      url_fetchers_.find(const_cast<URLFetcher*>(source));
  if (it == url_fetchers_.end()) {
    return;
  }
  std::string type_of_request(
      it->second.request_type == AutofillDownloadManager::REQUEST_QUERY ?
          "query" : "upload");
  const int kHttpResponseOk = 200;
  const int kHttpInternalServerError = 500;
  const int kHttpBadGateway = 502;
  const int kHttpServiceUnavailable = 503;

  CHECK(it->second.form_signatures.size());
  if (response_code != kHttpResponseOk) {
    bool back_off = false;
    std::string server_header;
    switch (response_code) {
      case kHttpBadGateway:
        if (!source->response_headers()->EnumerateHeader(NULL, "server",
                                                         &server_header) ||
            StartsWithASCII(server_header.c_str(),
                            AUTO_FILL_QUERY_SERVER_NAME_START_IN_HEADER,
                            false) != 0)
          break;
      case kHttpInternalServerError:
      case kHttpServiceUnavailable:
        back_off = true;
        break;
    }

    if (back_off) {
      base::Time back_off_time(base::Time::Now() + source->backoff_delay());
      if (it->second.request_type == AutofillDownloadManager::REQUEST_QUERY) {
        next_query_request_ = back_off_time;
      } else {
        next_upload_request_ = back_off_time;
      }
    }

    LOG(WARNING) << "AutofillDownloadManager: " << type_of_request
                 << " request has failed with response " << response_code;
    if (observer_) {
      observer_->OnHeuristicsRequestError(it->second.form_signatures[0],
                                          it->second.request_type,
                                          response_code);
    }
  } else {
    VLOG(1) << "AutofillDownloadManager: " << type_of_request
            << " request has succeeded";
    if (it->second.request_type == AutofillDownloadManager::REQUEST_QUERY) {
      CacheQueryRequest(it->second.form_signatures, data);
      if (observer_)
        observer_->OnLoadedAutofillHeuristics(data);
    } else {
      double new_positive_upload_rate = 0;
      double new_negative_upload_rate = 0;
      AutofillUploadXmlParser parse_handler(&new_positive_upload_rate,
                                            &new_negative_upload_rate);
      buzz::XmlParser parser(&parse_handler);
      parser.Parse(data.data(), data.length(), true);
      if (parse_handler.succeeded()) {
        SetPositiveUploadRate(new_positive_upload_rate);
        SetNegativeUploadRate(new_negative_upload_rate);
      }

      if (observer_)
        observer_->OnUploadedAutofillHeuristics(it->second.form_signatures[0]);
    }
  }
  delete it->first;
  url_fetchers_.erase(it);
}
