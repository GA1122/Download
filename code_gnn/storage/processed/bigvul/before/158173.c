  CreateDefaultRequestHandlerForNonNetworkService(
      net::URLRequestContextGetter* url_request_context_getter,
      storage::FileSystemContext* upload_file_system_context,
      ServiceWorkerNavigationHandleCore* service_worker_navigation_handle_core,
      AppCacheNavigationHandleCore* appcache_handle_core,
      bool was_request_intercepted) const {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
    DCHECK(started_);

    return base::BindOnce(
        &URLLoaderRequestController::CreateNonNetworkServiceURLLoader,
        weak_factory_.GetWeakPtr(),
        base::Unretained(url_request_context_getter),
        base::Unretained(upload_file_system_context),
        std::make_unique<NavigationRequestInfo>(*request_info_),
        base::Unretained(
            blink::ServiceWorkerUtils::IsServicificationEnabled() ||
                    was_request_intercepted
                ? nullptr
                : service_worker_navigation_handle_core),
        base::Unretained(was_request_intercepted ? nullptr
                                                 : appcache_handle_core));
  }
