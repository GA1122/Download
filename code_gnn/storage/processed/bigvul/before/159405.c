void DevToolsDataSource::StartRemoteDataRequest(
    const GURL& url,
    const content::URLDataSource::GotDataCallback& callback) {
  CHECK(url.is_valid());
  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("devtools_hard_coded_data_source", R"(
        semantics {
          sender: "Developer Tools Remote Data Request From Google"
          description:
            "This service fetches Chromium DevTools front-end files from the "
            "cloud for the remote debugging scenario."
          trigger:
            "When user attaches to mobile phone for debugging."
          data: "None"
          destination: GOOGLE_OWNED_SERVICE
        }
        policy {
          cookies_allowed: YES
          cookies_store: "user"
          setting: "This feature cannot be disabled by settings."
          chrome_policy {
            DeveloperToolsDisabled {
              policy_options {mode: MANDATORY}
              DeveloperToolsDisabled: true
            }
          }
        })");
  net::URLFetcher* fetcher = net::URLFetcher::Create(url, net::URLFetcher::GET,
                                                     this, traffic_annotation)
                                 .release();
  pending_[fetcher] = callback;
  fetcher->SetRequestContext(request_context_.get());
  fetcher->Start();
}
