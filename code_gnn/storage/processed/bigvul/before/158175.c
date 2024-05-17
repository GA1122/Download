  void CreateNonNetworkServiceURLLoader(
      net::URLRequestContextGetter* url_request_context_getter,
      storage::FileSystemContext* upload_file_system_context,
      std::unique_ptr<NavigationRequestInfo> request_info,
      ServiceWorkerNavigationHandleCore* service_worker_navigation_handle_core,
      AppCacheNavigationHandleCore* appcache_handle_core,
      const network::ResourceRequest&  ,
      network::mojom::URLLoaderRequest url_loader,
      network::mojom::URLLoaderClientPtr url_loader_client) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
    DCHECK(started_);

    default_loader_used_ = true;
    if (signed_exchange_utils::IsSignedExchangeHandlingEnabled()) {
      DCHECK(!network_loader_factory_);
      interceptors_.push_back(std::make_unique<SignedExchangeRequestHandler>(
          url::Origin::Create(request_info->common_params.url),
          request_info->common_params.url,
          GetURLLoaderOptions(request_info->is_main_frame),
          request_info->frame_tree_node_id,
          request_info->devtools_navigation_token,
          request_info->devtools_frame_token, request_info->report_raw_headers,
          request_info->begin_params->load_flags,
          base::MakeRefCounted<
              SignedExchangeURLLoaderFactoryForNonNetworkService>(
              resource_context_, url_request_context_getter),
          base::BindRepeating(
              &URLLoaderRequestController::CreateURLLoaderThrottles,
              base::Unretained(this))));
    }

    uint32_t options = GetURLLoaderOptions(request_info->is_main_frame);

    bool intercepted = false;
    if (g_interceptor.Get()) {
      intercepted = g_interceptor.Get().Run(
          &url_loader, frame_tree_node_id_, 0  , options,
          *resource_request_.get(), &url_loader_client,
          net::MutableNetworkTrafficAnnotationTag(
              kNavigationUrlLoaderTrafficAnnotation));
    }

    if (!intercepted && ResourceDispatcherHostImpl::Get()) {
      ResourceDispatcherHostImpl::Get()->BeginNavigationRequest(
          resource_context_, url_request_context_getter->GetURLRequestContext(),
          upload_file_system_context, *request_info,
          std::move(navigation_ui_data_), std::move(url_loader_client),
          std::move(url_loader), service_worker_navigation_handle_core,
          appcache_handle_core, options, global_request_id_);
    }

    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::UI},
        base::BindOnce(&NavigationURLLoaderImpl::OnRequestStarted, owner_,
                       base::TimeTicks::Now()));
  }
