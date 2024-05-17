void DevToolsDataSource::StartCustomDataRequest(
    const GURL& url,
    const content::URLDataSource::GotDataCallback& callback) {
  if (!url.is_valid()) {
    callback.Run(
        new base::RefCountedStaticMemory(kHttpNotFound, strlen(kHttpNotFound)));
    return;
  }
  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("devtools_free_data_source", R"(
        semantics {
          sender: "Developer Tools Remote Data Request"
          description:
            "This service fetches Chromium DevTools front-end files from the "
            "cloud for the remote debugging scenario. This can only happen if "
            "a URL was passed on the commandline via flag "
            "'--custom-devtools-frontend'. This URL overrides the default "
            "fetching from a Google website, see "
            "devtools_hard_coded_data_source."
          trigger:
            "When command line flag --custom-devtools-frontend is specified "
            "and DevTools is opened."
          data: "None"
          destination: WEBSITE
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
  fetcher->SetLoadFlags(net::LOAD_DISABLE_CACHE);
  fetcher->Start();
}
