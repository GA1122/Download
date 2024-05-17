  void StartWithoutNetworkService(
      net::URLRequestContextGetter* url_request_context_getter,
      storage::FileSystemContext* upload_file_system_context,
      ServiceWorkerNavigationHandleCore* service_worker_navigation_handle_core,
      AppCacheNavigationHandleCore* appcache_handle_core,
      std::unique_ptr<NavigationRequestInfo> request_info,
      std::unique_ptr<NavigationUIData> navigation_ui_data) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
    DCHECK(!started_);
    started_ = true;
    request_info_ = std::move(request_info);
    frame_tree_node_id_ = request_info_->frame_tree_node_id;
    web_contents_getter_ = base::BindRepeating(
        &GetWebContentsFromFrameTreeNodeID, frame_tree_node_id_);
    navigation_ui_data_ = std::move(navigation_ui_data);
    ResourceDispatcherHostImpl* rph = ResourceDispatcherHostImpl::Get();
    if (rph)
      global_request_id_ = rph->MakeGlobalRequestID();

    default_request_handler_factory_ = base::BindRepeating(
        &URLLoaderRequestController::
            CreateDefaultRequestHandlerForNonNetworkService,
        base::Unretained(this), base::Unretained(url_request_context_getter),
        base::Unretained(upload_file_system_context),
        base::Unretained(service_worker_navigation_handle_core),
        base::Unretained(appcache_handle_core));

    if (request_info_->common_params.url.SchemeIsBlob() &&
        request_info_->blob_url_loader_factory) {
      url_loader_ = ThrottlingURLLoader::CreateLoaderAndStart(
          network::SharedURLLoaderFactory::Create(
              std::move(request_info_->blob_url_loader_factory)),
          CreateURLLoaderThrottles(), -1  , 0  ,
          network::mojom::kURLLoadOptionNone, resource_request_.get(), this,
          kNavigationUrlLoaderTrafficAnnotation,
          base::ThreadTaskRunnerHandle::Get());
      return;
    }

    if (!blink::ServiceWorkerUtils::IsServicificationEnabled() ||
        !service_worker_navigation_handle_core) {
      url_loader_ = ThrottlingURLLoader::CreateLoaderAndStart(
          base::MakeRefCounted<SingleRequestURLLoaderFactory>(
              default_request_handler_factory_.Run(
                  false  )),
          CreateURLLoaderThrottles(), -1  , 0  ,
          network::mojom::kURLLoadOptionNone, resource_request_.get(),
          this  , kNavigationUrlLoaderTrafficAnnotation,
          base::ThreadTaskRunnerHandle::Get());
      return;
    }

    std::unique_ptr<NavigationLoaderInterceptor> service_worker_interceptor =
        CreateServiceWorkerInterceptor(*request_info_,
                                       service_worker_navigation_handle_core);
    if (!service_worker_interceptor) {
      url_loader_ = ThrottlingURLLoader::CreateLoaderAndStart(
          base::MakeRefCounted<SingleRequestURLLoaderFactory>(
              default_request_handler_factory_.Run(
                  false  )),
          CreateURLLoaderThrottles(), -1  , 0  ,
          network::mojom::kURLLoadOptionNone, resource_request_.get(),
          this  , kNavigationUrlLoaderTrafficAnnotation,
          base::ThreadTaskRunnerHandle::Get());
      return;
    }

    interceptors_.push_back(std::move(service_worker_interceptor));

    Restart();
  }
