void RenderFrameHostImpl::CommitNavigation(
    NavigationRequest* navigation_request,
    network::ResourceResponse* response,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    bool is_view_source,
    base::Optional<SubresourceLoaderParams> subresource_loader_params,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info,
    const base::UnguessableToken& devtools_navigation_token) {
  TRACE_EVENT2("navigation", "RenderFrameHostImpl::CommitNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               common_params.url.possibly_invalid_spec());
  DCHECK(!IsRendererDebugURL(common_params.url));
  DCHECK(
      (response && url_loader_client_endpoints) ||
      common_params.url.SchemeIs(url::kDataScheme) ||
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type) ||
      !IsURLHandledByNetworkStack(common_params.url));

  const bool is_first_navigation = !has_committed_any_navigation_;
  has_committed_any_navigation_ = true;

  UpdatePermissionsForNavigation(common_params, commit_params);

  ResetWaitingState();

  if (is_view_source && IsCurrent()) {
    DCHECK(!GetParent());
    render_view_host()->Send(new FrameMsg_EnableViewSourceMode(routing_id_));
  }

  const network::ResourceResponseHead head =
      response ? response->head : network::ResourceResponseHead();
  const bool is_same_document =
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type);

  std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
      subresource_loader_factories;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService) &&
      (!is_same_document || is_first_navigation)) {
    recreate_default_url_loader_factory_after_network_service_crash_ = false;
    subresource_loader_factories =
        std::make_unique<blink::URLLoaderFactoryBundleInfo>();
    BrowserContext* browser_context = GetSiteInstance()->GetBrowserContext();
    if (subresource_loader_params &&
        subresource_loader_params->appcache_loader_factory_info.is_valid()) {
      subresource_loader_factories->appcache_factory_info() =
          std::move(subresource_loader_params->appcache_loader_factory_info);

      if (!GetCreateNetworkFactoryCallbackForRenderFrame().is_null()) {
        network::mojom::URLLoaderFactoryPtrInfo original_factory =
            std::move(subresource_loader_factories->appcache_factory_info());
        network::mojom::URLLoaderFactoryRequest new_request = mojo::MakeRequest(
            &subresource_loader_factories->appcache_factory_info());
        GetCreateNetworkFactoryCallbackForRenderFrame().Run(
            std::move(new_request), GetProcess()->GetID(),
            std::move(original_factory));
      }
    }

    non_network_url_loader_factories_.clear();

    network::mojom::URLLoaderFactoryPtrInfo default_factory_info;

    std::string scheme = common_params.url.scheme();
    const auto& webui_schemes = URLDataManagerBackend::GetWebUISchemes();
    if (base::ContainsValue(webui_schemes, scheme)) {
      network::mojom::URLLoaderFactoryPtr factory_for_webui =
          CreateWebUIURLLoaderBinding(this, scheme);
      if ((enabled_bindings_ & kWebUIBindingsPolicyMask) &&
          !GetContentClient()->browser()->IsWebUIAllowedToMakeNetworkRequests(
              url::Origin::Create(common_params.url.GetOrigin()))) {
        default_factory_info = factory_for_webui.PassInterface();
        non_network_url_loader_factories_[url::kAboutScheme] =
            std::make_unique<AboutURLLoaderFactory>();
      } else {
        subresource_loader_factories->scheme_specific_factory_infos().emplace(
            scheme, factory_for_webui.PassInterface());
      }
    }

    if (!default_factory_info) {
      recreate_default_url_loader_factory_after_network_service_crash_ = true;
      bool bypass_redirect_checks =
          CreateNetworkServiceDefaultFactoryAndObserve(
              GetOriginForURLLoaderFactory(common_params),
              mojo::MakeRequest(&default_factory_info));
      subresource_loader_factories->set_bypass_redirect_checks(
          bypass_redirect_checks);
    }

    DCHECK(default_factory_info);
    subresource_loader_factories->default_factory_info() =
        std::move(default_factory_info);


    if (common_params.url.SchemeIsFile()) {
      auto file_factory = std::make_unique<FileURLLoaderFactory>(
          browser_context->GetPath(),
          browser_context->GetSharedCorsOriginAccessList(),
          base::CreateSequencedTaskRunnerWithTraits(
              {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
               base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
      non_network_url_loader_factories_.emplace(url::kFileScheme,
                                                std::move(file_factory));
    }

#if defined(OS_ANDROID)
    if (common_params.url.SchemeIs(url::kContentScheme)) {
      auto content_factory = std::make_unique<ContentURLLoaderFactory>(
          base::CreateSequencedTaskRunnerWithTraits(
              {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
               base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
      non_network_url_loader_factories_.emplace(url::kContentScheme,
                                                std::move(content_factory));
    }
#endif

    StoragePartition* partition =
        BrowserContext::GetStoragePartition(browser_context, GetSiteInstance());
    std::string storage_domain;
    if (site_instance_) {
      std::string partition_name;
      bool in_memory;
      GetContentClient()->browser()->GetStoragePartitionConfigForSite(
          browser_context, site_instance_->GetSiteURL(), true, &storage_domain,
          &partition_name, &in_memory);
    }
    non_network_url_loader_factories_.emplace(
        url::kFileSystemScheme,
        content::CreateFileSystemURLLoaderFactory(
            this,  false, partition->GetFileSystemContext(),
            storage_domain));

    non_network_url_loader_factories_.emplace(
        url::kDataScheme, std::make_unique<DataURLLoaderFactory>());

    GetContentClient()
        ->browser()
        ->RegisterNonNetworkSubresourceURLLoaderFactories(
            process_->GetID(), routing_id_, &non_network_url_loader_factories_);

    for (auto& factory : non_network_url_loader_factories_) {
      network::mojom::URLLoaderFactoryPtrInfo factory_proxy_info;
      auto factory_request = mojo::MakeRequest(&factory_proxy_info);
      GetContentClient()->browser()->WillCreateURLLoaderFactory(
          browser_context, this, GetProcess()->GetID(),
          false  , false  ,
          GetOriginForURLLoaderFactory(common_params), &factory_request,
          nullptr  , nullptr  );
      devtools_instrumentation::WillCreateURLLoaderFactory(
          this, false  , false  ,
          &factory_request);
      factory.second->Clone(std::move(factory_request));
      subresource_loader_factories->scheme_specific_factory_infos().emplace(
          factory.first, std::move(factory_proxy_info));
    }

    subresource_loader_factories->initiator_specific_factory_infos() =
        CreateInitiatorSpecificURLLoaderFactories(
            initiators_requiring_separate_url_loader_factory_);
  }

  DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         is_same_document || !is_first_navigation ||
         subresource_loader_factories);

  if (is_same_document) {
    DCHECK(same_document_navigation_request_);
    GetNavigationControl()->CommitSameDocumentNavigation(
        common_params, commit_params,
        base::BindOnce(&RenderFrameHostImpl::OnSameDocumentCommitProcessed,
                       base::Unretained(this),
                       same_document_navigation_request_->navigation_handle()
                           ->GetNavigationId(),
                       common_params.should_replace_current_entry));
  } else {
    blink::mojom::ControllerServiceWorkerInfoPtr controller;
    blink::mojom::ServiceWorkerObjectAssociatedPtrInfo remote_object;
    blink::mojom::ServiceWorkerState sent_state;
    if (subresource_loader_params &&
        subresource_loader_params->controller_service_worker_info) {
      controller =
          std::move(subresource_loader_params->controller_service_worker_info);
      if (controller->object_info) {
        controller->object_info->request = mojo::MakeRequest(&remote_object);
        sent_state = controller->object_info->state;
      }
    }

    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        factory_bundle_for_prefetch;
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory;
    if (subresource_loader_factories) {
      auto bundle = base::MakeRefCounted<blink::URLLoaderFactoryBundle>(
          std::move(subresource_loader_factories));
      subresource_loader_factories = CloneFactoryBundle(bundle);
      factory_bundle_for_prefetch = CloneFactoryBundle(bundle);
    } else if (!is_same_document || is_first_navigation) {
      DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
      factory_bundle_for_prefetch =
          std::make_unique<blink::URLLoaderFactoryBundleInfo>();
      network::mojom::URLLoaderFactoryPtrInfo factory_info;
      CreateNetworkServiceDefaultFactoryInternal(
          url::Origin(), mojo::MakeRequest(&factory_info));
      factory_bundle_for_prefetch->default_factory_info() =
          std::move(factory_info);
    }

    if (factory_bundle_for_prefetch) {
      auto* storage_partition = static_cast<StoragePartitionImpl*>(
          BrowserContext::GetStoragePartition(
              GetSiteInstance()->GetBrowserContext(), GetSiteInstance()));
      base::PostTaskWithTraits(
          FROM_HERE, {BrowserThread::IO},
          base::BindOnce(&PrefetchURLLoaderService::GetFactory,
                         storage_partition->GetPrefetchURLLoaderService(),
                         mojo::MakeRequest(&prefetch_loader_factory),
                         frame_tree_node_->frame_tree_node_id(),
                         std::move(factory_bundle_for_prefetch)));
    }

    mojom::NavigationClient* navigation_client = nullptr;
    if (IsPerNavigationMojoInterfaceEnabled() && navigation_request)
      navigation_client = navigation_request->GetCommitNavigationClient();

    if (!GetParent() && frame_tree_node()->current_frame_host() == this) {
      if (NavigationEntryImpl* last_committed_entry =
              NavigationEntryImpl::FromNavigationEntry(
                  frame_tree_node()
                      ->navigator()
                      ->GetController()
                      ->GetLastCommittedEntry())) {
        if (last_committed_entry->back_forward_cache_metrics()) {
          last_committed_entry->back_forward_cache_metrics()
              ->RecordFeatureUsage(this);
        }
      }
    }

    SendCommitNavigation(
        navigation_client, navigation_request, head, common_params,
        commit_params, std::move(url_loader_client_endpoints),
        std::move(subresource_loader_factories),
        std::move(subresource_overrides), std::move(controller),
        std::move(provider_info), std::move(prefetch_loader_factory),
        devtools_navigation_token);

    if (remote_object.is_valid()) {
      base::PostTaskWithTraits(
          FROM_HERE, {BrowserThread::IO},
          base::BindOnce(
              &ServiceWorkerObjectHost::AddRemoteObjectPtrAndUpdateState,
              subresource_loader_params->controller_service_worker_object_host,
              std::move(remote_object), sent_state));
    }

    if (IsURLHandledByNetworkStack(common_params.url))
      last_navigation_previews_state_ = common_params.previews_state;
  }

  is_loading_ = true;
}
