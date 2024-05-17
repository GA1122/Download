void RenderFrameImpl::CommitNavigationInternal(
    const network::ResourceResponseHead& head,
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory,
    const base::UnguessableToken& devtools_navigation_token,
    mojom::FrameNavigationControl::CommitNavigationCallback callback,
    mojom::NavigationClient::CommitNavigationCallback
        per_navigation_mojo_interface_callback) {
  DCHECK(!IsRendererDebugURL(common_params.url));
  DCHECK(
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type));
  if (ShouldIgnoreCommitNavigation(commit_params)) {
    browser_side_navigation_pending_url_ = GURL();
    AbortCommitNavigation(std::move(callback),
                          blink::mojom::CommitResult::Aborted);
    return;
  }

  bool was_initiated_in_this_frame = false;
  if (IsPerNavigationMojoInterfaceEnabled()) {
    was_initiated_in_this_frame =
        navigation_client_impl_ &&
        navigation_client_impl_->was_initiated_in_this_frame();
  } else {
    was_initiated_in_this_frame =
        browser_side_navigation_pending_ &&
        browser_side_navigation_pending_url_ == commit_params.original_url &&
        commit_params.nav_entry_id == 0;
  }

  DCHECK(common_params.url.SchemeIs(url::kJavaScriptScheme) ||
         !base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         subresource_loader_factories);

  const network::ResourceResponseHead* response_head = nullptr;
  if (!frame_->Parent() && !frame_->IsViewSourceModeEnabled())
    response_head = &head;
  int request_id = ResourceDispatcher::MakeRequestID();
  std::unique_ptr<DocumentState> document_state = BuildDocumentStateFromParams(
      common_params, commit_params, base::TimeTicks::Now(), std::move(callback),
      std::move(per_navigation_mojo_interface_callback), response_head,
      std::move(navigation_client_impl_), request_id,
      was_initiated_in_this_frame);

  bool is_client_redirect =
      !!(common_params.transition & ui::PAGE_TRANSITION_CLIENT_REDIRECT);
  auto navigation_params =
      std::make_unique<WebNavigationParams>(devtools_navigation_token);
  navigation_params->is_client_redirect = is_client_redirect;
  FillMiscNavigationParams(common_params, commit_params,
                           navigation_params.get());

  auto commit_with_params = base::BindOnce(
      &RenderFrameImpl::CommitNavigationWithParams, weak_factory_.GetWeakPtr(),
      common_params, commit_params, std::move(subresource_loader_factories),
      std::move(subresource_overrides),
      std::move(controller_service_worker_info), std::move(provider_info),
      std::move(prefetch_loader_factory), std::move(document_state));

  bool should_load_data_url = !common_params.base_url_for_data_url.is_empty();
#if defined(OS_ANDROID)
  should_load_data_url |= !commit_params.data_url_as_string.empty();
#endif
  if (is_main_frame_ && should_load_data_url) {
    std::string mime_type, charset, data;
    GURL base_url;
    DecodeDataURL(common_params, commit_params, &mime_type, &charset, &data,
                  &base_url);
    navigation_params->url = base_url;
    WebNavigationParams::FillStaticResponse(navigation_params.get(),
                                            WebString::FromUTF8(mime_type),
                                            WebString::FromUTF8(charset), data);
    navigation_params->unreachable_url = common_params.history_url_for_data_url;
    std::move(commit_with_params).Run(std::move(navigation_params));
    return;
  }

  FillNavigationParamsRequest(common_params, commit_params,
                              navigation_params.get());
  if (!url_loader_client_endpoints &&
      common_params.url.SchemeIs(url::kDataScheme)) {
    std::string mime_type, charset, data;
    if (!net::DataURL::Parse(common_params.url, &mime_type, &charset, &data)) {
      CHECK(false) << "Invalid URL passed: "
                   << common_params.url.possibly_invalid_spec();
      return;
    }
    WebNavigationParams::FillStaticResponse(navigation_params.get(),
                                            WebString::FromUTF8(mime_type),
                                            WebString::FromUTF8(charset), data);
  } else {
    NavigationBodyLoader::FillNavigationParamsResponseAndBodyLoader(
        common_params, commit_params, request_id, head,
        std::move(url_loader_client_endpoints),
        GetTaskRunner(blink::TaskType::kInternalLoading), GetRoutingID(),
        !frame_->Parent(), navigation_params.get());
  }

  bool is_mhtml_archive =
      base::LowerCaseEqualsASCII(head.mime_type, "multipart/related") ||
      base::LowerCaseEqualsASCII(head.mime_type, "message/rfc822");
  if (is_mhtml_archive && navigation_params->body_loader) {
    mhtml_body_loader_client_ =
        std::make_unique<RenderFrameImpl::MHTMLBodyLoaderClient>(
            std::move(navigation_params), std::move(commit_with_params));
    return;
  }

  std::move(commit_with_params).Run(std::move(navigation_params));
}
