void RenderFrameHostImpl::CommitNavigation(
    network::ResourceResponse* response,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<StreamHandle> body,
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool is_view_source,
    base::Optional<SubresourceLoaderParams> subresource_loader_params,
    const base::UnguessableToken& devtools_navigation_token) {
  TRACE_EVENT2("navigation", "RenderFrameHostImpl::CommitNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               common_params.url.possibly_invalid_spec());
  DCHECK(!IsRendererDebugURL(common_params.url));
  DCHECK(
      (response && (url_loader_client_endpoints || body)) ||
      common_params.url.SchemeIs(url::kDataScheme) ||
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type) ||
      !IsURLHandledByNetworkStack(common_params.url));

  const bool is_first_navigation = !has_committed_any_navigation_;
  has_committed_any_navigation_ = true;

  UpdatePermissionsForNavigation(common_params, request_params);

  ResetWaitingState();

  if (is_view_source && IsCurrent()) {
    DCHECK(!GetParent());
    render_view_host()->Send(new FrameMsg_EnableViewSourceMode(routing_id_));
  }

  const GURL body_url = body.get() ? body->GetURL() : GURL();
  const network::ResourceResponseHead head =
      response ? response->head : network::ResourceResponseHead();
  const bool is_same_document =
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type);

  std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories;
  mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService) &&
      (!is_same_document || is_first_navigation)) {
    subresource_loader_factories =
        std::make_unique<URLLoaderFactoryBundleInfo>();
    network::mojom::URLLoaderFactoryPtrInfo default_factory_info;
    StoragePartitionImpl* storage_partition =
        static_cast<StoragePartitionImpl*>(BrowserContext::GetStoragePartition(
            GetSiteInstance()->GetBrowserContext(), GetSiteInstance()));
    if (subresource_loader_params &&
        subresource_loader_params->loader_factory_info.is_valid()) {
      default_factory_info =
          std::move(subresource_loader_params->loader_factory_info);
    } else {
      std::string scheme = common_params.url.scheme();
      const auto& schemes = URLDataManagerBackend::GetWebUISchemes();
      if (std::find(schemes.begin(), schemes.end(), scheme) != schemes.end()) {
        network::mojom::URLLoaderFactoryPtr factory_for_webui =
            CreateWebUIURLLoader(this, scheme);
        if (enabled_bindings_ & BINDINGS_POLICY_WEB_UI) {
          default_factory_info = factory_for_webui.PassInterface();
        } else {
          subresource_loader_factories->factories_info().emplace(
              scheme, factory_for_webui.PassInterface());
        }
      }
    }

    if (!default_factory_info) {
      CreateNetworkServiceDefaultFactoryAndObserve(
          mojo::MakeRequest(&default_factory_info));
    }

    DCHECK(default_factory_info);
    subresource_loader_factories->default_factory_info() =
        std::move(default_factory_info);
    network::mojom::URLLoaderFactoryPtrInfo blob_factory_info;
    storage_partition->GetBlobURLLoaderFactory()->HandleRequest(
        mojo::MakeRequest(&blob_factory_info));
    subresource_loader_factories->factories_info().emplace(
        url::kBlobScheme, std::move(blob_factory_info));

    non_network_url_loader_factories_.clear();

    if (common_params.url.SchemeIsFile()) {
      auto file_factory = std::make_unique<FileURLLoaderFactory>(
          GetProcess()->GetBrowserContext()->GetPath(),
          base::CreateSequencedTaskRunnerWithTraits(
              {base::MayBlock(), base::TaskPriority::BACKGROUND,
               base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
      non_network_url_loader_factories_.emplace(url::kFileScheme,
                                                std::move(file_factory));
    }

    GetContentClient()
        ->browser()
        ->RegisterNonNetworkSubresourceURLLoaderFactories(
            this, common_params.url, &non_network_url_loader_factories_);

    for (auto& factory : non_network_url_loader_factories_) {
      network::mojom::URLLoaderFactoryPtrInfo factory_proxy_info;
      factory.second->Clone(mojo::MakeRequest(&factory_proxy_info));
      subresource_loader_factories->factories_info().emplace(
          factory.first, std::move(factory_proxy_info));
    }

    if (subresource_loader_params) {
      controller_service_worker_info =
          std::move(subresource_loader_params->controller_service_worker_info);
    }
  }

  DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         is_same_document || !is_first_navigation ||
         subresource_loader_factories);

  GetNavigationControl()->CommitNavigation(
      head, body_url, common_params, request_params,
      std::move(url_loader_client_endpoints),
      std::move(subresource_loader_factories),
      std::move(controller_service_worker_info), devtools_navigation_token);

  if (IsURLHandledByNetworkStack(common_params.url) &&
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type)) {
    last_navigation_previews_state_ = common_params.previews_state;
  }

  if (!is_same_document) {
    stream_handle_ = std::move(body);
  }

  pending_commit_ = true;
  is_loading_ = true;
}
