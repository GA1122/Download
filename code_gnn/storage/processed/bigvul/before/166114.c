void RenderFrameHostImpl::BeginNavigation(
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    blink::mojom::BlobURLTokenPtr blob_url_token,
    mojom::NavigationClientAssociatedPtrInfo navigation_client,
    blink::mojom::NavigationInitiatorPtr navigation_initiator) {
  if (is_attaching_inner_delegate_) {
    return;
  }

  if (!is_active())
    return;

  TRACE_EVENT2("navigation", "RenderFrameHostImpl::BeginNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               common_params.url.possibly_invalid_spec());

  DCHECK(IsPerNavigationMojoInterfaceEnabled() == navigation_client.is_valid());

  CommonNavigationParams validated_params = common_params;
  GetProcess()->FilterURL(false, &validated_params.url);
  if (!validated_params.base_url_for_data_url.is_empty()) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_BASE_URL_FOR_DATA_URL_SPECIFIED);
    return;
  }

  GetProcess()->FilterURL(true, &begin_params->searchable_form_url);

  if (!ChildProcessSecurityPolicyImpl::GetInstance()->CanReadRequestBody(
          GetSiteInstance(), validated_params.post_data)) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_ILLEGAL_UPLOAD_PARAMS);
    return;
  }

  if (validated_params.url.SchemeIs(kChromeErrorScheme)) {
    mojo::ReportBadMessage("Renderer cannot request error page URLs directly");
    return;
  }

  if (common_params.url.SchemeIsBlob() && !validated_params.url.SchemeIsBlob())
    blob_url_token = nullptr;

  if (blob_url_token && !validated_params.url.SchemeIsBlob()) {
    mojo::ReportBadMessage("Blob URL Token, but not a blob: URL");
    return;
  }
  scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory;
  if (blob_url_token) {
    blob_url_loader_factory =
        ChromeBlobStorageContext::URLLoaderFactoryForToken(
            GetSiteInstance()->GetBrowserContext(), std::move(blob_url_token));
  }

  if (blink::BlobUtils::MojoBlobURLsEnabled() &&
      validated_params.url.SchemeIsBlob() && !blob_url_loader_factory) {
    blob_url_loader_factory = ChromeBlobStorageContext::URLLoaderFactoryForUrl(
        GetSiteInstance()->GetBrowserContext(), validated_params.url);
  }

  if (waiting_for_init_) {
    pending_navigate_ = std::make_unique<PendingNavigation>(
        validated_params, std::move(begin_params),
        std::move(blob_url_loader_factory), std::move(navigation_client),
        std::move(navigation_initiator));
    return;
  }

  frame_tree_node()->navigator()->OnBeginNavigation(
      frame_tree_node(), validated_params, std::move(begin_params),
      std::move(blob_url_loader_factory), std::move(navigation_client),
      std::move(navigation_initiator));
}
