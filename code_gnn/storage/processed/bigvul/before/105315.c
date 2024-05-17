bool AutofillDownloadManager::StartRequest(
    const std::string& form_xml,
    const FormRequestData& request_data) {
  net::URLRequestContextGetter* request_context =
      Profile::GetDefaultRequestContext();
  if (!request_context)
    return false;
  std::string request_url;
  if (request_data.request_type == AutofillDownloadManager::REQUEST_QUERY)
    request_url = AUTO_FILL_QUERY_SERVER_REQUEST_URL;
  else
    request_url = AUTO_FILL_UPLOAD_SERVER_REQUEST_URL;

  URLFetcher *fetcher = URLFetcher::Create(fetcher_id_for_unittest_++,
                                           GURL(request_url),
                                           URLFetcher::POST,
                                           this);
  url_fetchers_[fetcher] = request_data;
  fetcher->set_automatically_retry_on_5xx(false);
  fetcher->set_request_context(request_context);
  fetcher->set_upload_data("text/plain", form_xml);
  fetcher->Start();
  return true;
}
