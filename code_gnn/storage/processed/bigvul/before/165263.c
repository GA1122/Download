std::unique_ptr<NavigationRequest> NavigationRequest::CreateForCommit(
    FrameTreeNode* frame_tree_node,
    RenderFrameHostImpl* render_frame_host,
    NavigationEntryImpl* entry,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    bool is_renderer_initiated,
    bool is_same_document) {
  CommonNavigationParams common_params(
      params.url,
      params.origin, params.referrer, params.transition,
      is_same_document ? FrameMsg_Navigate_Type::SAME_DOCUMENT
                       : FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT,
      NavigationDownloadPolicy::kAllow, params.should_replace_current_entry,
      params.base_url, params.base_url, PREVIEWS_UNSPECIFIED,
      base::TimeTicks::Now(), params.method, nullptr,
      base::Optional<SourceLocation>(), false  ,
      params.gesture == NavigationGestureUser, InitiatorCSPInfo(),
      std::string()  , base::TimeTicks::Now());
  CommitNavigationParams commit_params(
      params.origin, params.is_overriding_user_agent, params.redirects,
      params.original_request_url, params.method,
      false  , params.page_state,
      params.nav_entry_id, false  ,
      std::map<std::string, bool>()  ,
      params.intended_as_new_entry, -1  ,
      -1  ,
      -1  , false  ,
      params.history_list_was_cleared);
  mojom::BeginNavigationParamsPtr begin_params =
      mojom::BeginNavigationParams::New();
  std::unique_ptr<NavigationRequest> navigation_request(new NavigationRequest(
      frame_tree_node, common_params, std::move(begin_params), commit_params,
      !is_renderer_initiated, false  ,
      true  ,
      entry ? entry->GetFrameEntry(frame_tree_node) : nullptr, entry,
      nullptr  ,
      mojom::NavigationClientAssociatedPtrInfo(),
      blink::mojom::NavigationInitiatorPtr()));

  navigation_request->state_ = RESPONSE_STARTED;
  navigation_request->render_frame_host_ = render_frame_host;
  navigation_request->CreateNavigationHandle(true);
  DCHECK(navigation_request->navigation_handle());
  return navigation_request;
}
