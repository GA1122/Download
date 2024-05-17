NavigationURLLoaderImpl::NavigationURLLoaderImpl(
    ResourceContext* resource_context,
    StoragePartition* storage_partition,
    std::unique_ptr<NavigationRequestInfo> request_info,
    std::unique_ptr<NavigationUIData> navigation_ui_data,
    ServiceWorkerNavigationHandle* service_worker_navigation_handle,
    AppCacheNavigationHandle* appcache_handle,
    NavigationURLLoaderDelegate* delegate,
    std::vector<std::unique_ptr<NavigationLoaderInterceptor>>
        initial_interceptors)
    : delegate_(delegate),
      allow_download_(request_info->common_params.allow_download),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  int frame_tree_node_id = request_info->frame_tree_node_id;

  TRACE_EVENT_ASYNC_BEGIN_WITH_TIMESTAMP1(
      "navigation", "Navigation timeToResponseStarted", this,
      request_info->common_params.navigation_start, "FrameTreeNode id",
      frame_tree_node_id);

  ServiceWorkerNavigationHandleCore* service_worker_navigation_handle_core =
      service_worker_navigation_handle
          ? service_worker_navigation_handle->core()
          : nullptr;

  AppCacheNavigationHandleCore* appcache_handle_core =
      appcache_handle ? appcache_handle->core() : nullptr;

  std::unique_ptr<network::ResourceRequest> new_request = CreateResourceRequest(
      request_info.get(), frame_tree_node_id, allow_download_);
  new_request->transition_type = request_info->common_params.transition;

  if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    DCHECK(!request_controller_);
    request_controller_ = std::make_unique<URLLoaderRequestController>(
         
        std::vector<std::unique_ptr<NavigationLoaderInterceptor>>(),
        std::move(new_request), resource_context,
        request_info->common_params.url, request_info->is_main_frame,
          nullptr,
          nullptr, std::set<std::string>(),
          false, weak_factory_.GetWeakPtr());

    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(
            &URLLoaderRequestController::StartWithoutNetworkService,
            base::Unretained(request_controller_.get()),
            base::RetainedRef(storage_partition->GetURLRequestContext()),
            base::Unretained(storage_partition->GetFileSystemContext()),
            base::Unretained(service_worker_navigation_handle_core),
            base::Unretained(appcache_handle_core), std::move(request_info),
            std::move(navigation_ui_data)));
    return;
  }

  FrameTreeNode* frame_tree_node =
      FrameTreeNode::GloballyFindByID(frame_tree_node_id);
  network::mojom::URLLoaderFactoryPtrInfo factory_for_webui;
  const auto& schemes = URLDataManagerBackend::GetWebUISchemes();
  std::string scheme = new_request->url.scheme();
  if (base::ContainsValue(schemes, scheme)) {
    factory_for_webui = CreateWebUIURLLoaderBinding(
                            frame_tree_node->current_frame_host(), scheme)
                            .PassInterface();
  }

  network::mojom::URLLoaderFactoryPtrInfo proxied_factory_info;
  network::mojom::URLLoaderFactoryRequest proxied_factory_request;
  bool bypass_redirect_checks = false;
  auto* partition = static_cast<StoragePartitionImpl*>(storage_partition);
  if (frame_tree_node) {
    GetContentClient()
        ->browser()
        ->RegisterNonNetworkNavigationURLLoaderFactories(
            frame_tree_node_id, &non_network_url_loader_factories_);

    url::Origin navigation_request_initiator = url::Origin();
    network::mojom::URLLoaderFactoryPtrInfo factory_info;
    auto factory_request = mojo::MakeRequest(&factory_info);
    bool use_proxy = GetContentClient()->browser()->WillCreateURLLoaderFactory(
        partition->browser_context(), frame_tree_node->current_frame_host(),
        true  , navigation_request_initiator,
        &factory_request, &bypass_redirect_checks);
    if (RenderFrameDevToolsAgentHost::WillCreateURLLoaderFactory(
            frame_tree_node->current_frame_host(), true, false,
            &factory_request)) {
      use_proxy = true;
    }
    if (use_proxy) {
      proxied_factory_request = std::move(factory_request);
      proxied_factory_info = std::move(factory_info);
    }

    const std::string storage_domain;
    non_network_url_loader_factories_[url::kFileSystemScheme] =
        CreateFileSystemURLLoaderFactory(frame_tree_node->current_frame_host(),
                                          true,
                                         partition->GetFileSystemContext(),
                                         storage_domain);
  }

  non_network_url_loader_factories_[url::kFileScheme] =
      std::make_unique<FileURLLoaderFactory>(
          partition->browser_context()->GetPath(),
          base::CreateSequencedTaskRunnerWithTraits(
              {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
               base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
  std::set<std::string> known_schemes;
  for (auto& iter : non_network_url_loader_factories_)
    known_schemes.insert(iter.first);

  DCHECK(!request_controller_);
  request_controller_ = std::make_unique<URLLoaderRequestController>(
      std::move(initial_interceptors), std::move(new_request), resource_context,
      request_info->common_params.url, request_info->is_main_frame,
      std::move(proxied_factory_request), std::move(proxied_factory_info),
      std::move(known_schemes), bypass_redirect_checks,
      weak_factory_.GetWeakPtr());
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::IO},
      base::BindOnce(
          &URLLoaderRequestController::Start,
          base::Unretained(request_controller_.get()),
          partition->url_loader_factory_getter()->GetNetworkFactoryInfo(),
          service_worker_navigation_handle_core, appcache_handle_core,
          std::move(request_info), std::move(navigation_ui_data),
          std::move(factory_for_webui), frame_tree_node_id,
          ServiceManagerConnection::GetForProcess()->GetConnector()->Clone()));
}
