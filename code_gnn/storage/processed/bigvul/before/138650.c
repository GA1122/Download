void RenderFrameHostImpl::CommitNavigation(
    ResourceResponse* response,
    std::unique_ptr<StreamHandle> body,
    mojo::ScopedDataPipeConsumerHandle handle,
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool is_view_source) {
  DCHECK(
      (response && (body.get() || handle.is_valid())) ||
      common_params.url.SchemeIs(url::kDataScheme) ||
      !ShouldMakeNetworkRequestForURL(common_params.url) ||
      FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type) ||
      IsRendererDebugURL(common_params.url));
  UpdatePermissionsForNavigation(common_params, request_params);

  ResetWaitingState();

  if (is_view_source &&
      this == frame_tree_node_->render_manager()->current_frame_host()) {
    DCHECK(!GetParent());
    render_view_host()->Send(new FrameMsg_EnableViewSourceMode(routing_id_));
  }

  const GURL body_url = body.get() ? body->GetURL() : GURL();
  const ResourceResponseHead head = response ?
      response->head : ResourceResponseHead();
  Send(new FrameMsg_CommitNavigation(routing_id_, head, body_url,
                                     handle.release(), common_params,
                                     request_params));

  if (ShouldMakeNetworkRequestForURL(common_params.url) &&
      !FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type)) {
    last_navigation_previews_state_ = common_params.previews_state;
  }

  stream_handle_ = std::move(body);

  if (!IsRendererDebugURL(common_params.url)) {
    pending_commit_ = true;
    is_loading_ = true;
  }
}
