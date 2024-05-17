void DevToolsUIBindings::LoadNetworkResource(const DispatchCallback& callback,
                                             const std::string& url,
                                             const std::string& headers,
                                             int stream_id) {
  GURL gurl(url);
  if (!gurl.is_valid()) {
    base::DictionaryValue response;
    response.SetInteger("statusCode", 404);
    callback.Run(&response);
    return;
  }
  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("devtools_network_resource", R"(
        semantics {
          sender: "Developer Tools"
          description:
            "When user opens Developer Tools, the browser may fetch additional "
            "resources from the network to enrich the debugging experience "
            "(e.g. source map resources)."
          trigger: "User opens Developer Tools to debug a web page."
          data: "Any resources requested by Developer Tools."
          destination: WEBSITE
        }
        policy {
          cookies_allowed: YES
          cookies_store: "user"
          setting:
            "It's not possible to disable this feature from settings."
          chrome_policy {
            DeveloperToolsDisabled {
              policy_options {mode: MANDATORY}
              DeveloperToolsDisabled: true
            }
          }
        })");

  net::URLFetcher* fetcher = net::URLFetcher::Create(gurl, net::URLFetcher::GET,
                                                     this, traffic_annotation)
                                 .release();
  pending_requests_[fetcher] = callback;
  fetcher->SetRequestContext(profile_->GetRequestContext());
  fetcher->SetExtraRequestHeaders(headers);
  fetcher->SaveResponseWithWriter(
      std::unique_ptr<net::URLFetcherResponseWriter>(
          new ResponseWriter(weak_factory_.GetWeakPtr(), stream_id)));
  fetcher->Start();
}
