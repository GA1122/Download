ChromeContentBrowserClient::CreateURLLoaderThrottles(
    const network::ResourceRequest& request,
    content::ResourceContext* resource_context,
    const base::RepeatingCallback<content::WebContents*()>& wc_getter,
    content::NavigationUIData* navigation_ui_data,
    int frame_tree_node_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  bool network_service_enabled =
      base::FeatureList::IsEnabled(network::features::kNetworkService);
  std::vector<std::unique_ptr<content::URLLoaderThrottle>> result;

  ProfileIOData* io_data = nullptr;
  if (safe_browsing_service_ ||
      data_reduction_proxy::params::IsEnabledWithNetworkService()) {
    io_data = ProfileIOData::FromResourceContext(resource_context);
  }

  if (io_data && io_data->data_reduction_proxy_io_data() &&
      data_reduction_proxy::params::IsEnabledWithNetworkService()) {
    net::HttpRequestHeaders headers;
    data_reduction_proxy::DataReductionProxyRequestOptions* request_options =
        io_data->data_reduction_proxy_io_data()->request_options();
    request_options->AddPageIDRequestHeader(&headers,
                                            request_options->GeneratePageId());
    result.push_back(std::make_unique<
                     data_reduction_proxy::DataReductionProxyURLLoaderThrottle>(
        headers,
        io_data->data_reduction_proxy_io_data()->GetThrottleManager()));
  }

#if defined(SAFE_BROWSING_DB_LOCAL) || defined(SAFE_BROWSING_DB_REMOTE)
  if (io_data && safe_browsing_service_) {
    bool matches_enterprise_whitelist = safe_browsing::IsURLWhitelistedByPolicy(
        request.url, io_data->safe_browsing_whitelist_domains());
    if (!matches_enterprise_whitelist &&
        (network_service_enabled ||
         base::FeatureList::IsEnabled(
             safe_browsing::kCheckByURLLoaderThrottle))) {
      auto* delegate = GetSafeBrowsingUrlCheckerDelegate(resource_context);
      if (delegate && !delegate->ShouldSkipRequestCheck(
                          resource_context, request.url, frame_tree_node_id,
                          -1  , -1  ,
                          request.originated_from_service_worker)) {
        auto safe_browsing_throttle =
            safe_browsing::BrowserURLLoaderThrottle::MaybeCreate(delegate,
                                                                 wc_getter);
        if (safe_browsing_throttle)
          result.push_back(std::move(safe_browsing_throttle));
      }
    }
  }
#endif   

  ChromeNavigationUIData* chrome_navigation_ui_data =
      static_cast<ChromeNavigationUIData*>(navigation_ui_data);
  if (chrome_navigation_ui_data &&
      chrome_navigation_ui_data->prerender_mode() != prerender::NO_PRERENDER) {
    result.push_back(std::make_unique<prerender::PrerenderURLLoaderThrottle>(
        chrome_navigation_ui_data->prerender_mode(),
        chrome_navigation_ui_data->prerender_histogram_prefix(),
        base::BindOnce(GetPrerenderCanceller, wc_getter),
        base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::UI})));
  }

  if (io_data) {
    bool is_off_the_record = io_data->IsOffTheRecord();
    bool is_signed_in =
        !is_off_the_record &&
        !io_data->google_services_account_id()->GetValue().empty();

    chrome::mojom::DynamicParams dynamic_params = {
        io_data->force_google_safesearch()->GetValue(),
        io_data->force_youtube_restrict()->GetValue(),
        io_data->allowed_domains_for_apps()->GetValue(),
        variations::VariationsHttpHeaderProvider::GetInstance()
            ->GetClientDataHeader(is_signed_in)};
    result.push_back(std::make_unique<GoogleURLLoaderThrottle>(
        is_off_the_record, std::move(dynamic_params)));

    if (network_service_enabled) {
      result.push_back(std::make_unique<ProtocolHandlerThrottle>(
          io_data->protocol_handler_registry_io_thread_delegate()));
    }
  }

#if BUILDFLAG(ENABLE_PLUGINS)
  if (blink::ServiceWorkerUtils::IsServicificationEnabled()) {
    result.push_back(
        std::make_unique<PluginResponseInterceptorURLLoaderThrottle>(
            resource_context, request.resource_type, frame_tree_node_id));
  }
#endif

  if (network_service_enabled) {
    auto delegate = std::make_unique<signin::HeaderModificationDelegateImpl>(
        resource_context);
    auto signin_throttle = signin::URLLoaderThrottle::MaybeCreate(
        std::move(delegate), navigation_ui_data, wc_getter);
    if (signin_throttle)
      result.push_back(std::move(signin_throttle));
  }
  return result;
}
