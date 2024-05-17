NavigationRequest::NavigationRequest(
    FrameTreeNode* frame_tree_node,
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    const RequestNavigationParams& request_params,
    bool browser_initiated,
    bool from_begin_navigation,
    const FrameNavigationEntry* frame_entry,
    const NavigationEntryImpl* entry,
    std::unique_ptr<NavigationUIData> navigation_ui_data)
    : frame_tree_node_(frame_tree_node),
      common_params_(common_params),
      begin_params_(std::move(begin_params)),
      request_params_(request_params),
      browser_initiated_(browser_initiated),
      navigation_ui_data_(std::move(navigation_ui_data)),
      state_(NOT_STARTED),
      restore_type_(RestoreType::NONE),
      is_view_source_(false),
      bindings_(NavigationEntryImpl::kInvalidBindings),
      response_should_be_rendered_(true),
      associated_site_instance_type_(AssociatedSiteInstanceType::NONE),
      from_begin_navigation_(from_begin_navigation),
      has_stale_copy_in_cache_(false),
      net_error_(net::OK),
      devtools_navigation_token_(base::UnguessableToken::Create()),
      weak_factory_(this) {
  DCHECK(!browser_initiated || (entry != nullptr && frame_entry != nullptr));
  DCHECK(!IsRendererDebugURL(common_params_.url));
  TRACE_EVENT_ASYNC_BEGIN2("navigation", "NavigationRequest", this,
                           "frame_tree_node",
                           frame_tree_node_->frame_tree_node_id(), "url",
                           common_params_.url.possibly_invalid_spec());

  common_params_.referrer =
      Referrer::SanitizeForRequest(common_params_.url, common_params_.referrer);

  if (from_begin_navigation_) {
    source_site_instance_ =
        frame_tree_node->current_frame_host()->GetSiteInstance();
  } else {
    FrameNavigationEntry* frame_navigation_entry =
        entry->GetFrameEntry(frame_tree_node);
    if (frame_navigation_entry) {
      source_site_instance_ = frame_navigation_entry->source_site_instance();
      dest_site_instance_ = frame_navigation_entry->site_instance();
    }
    restore_type_ = entry->restore_type();
    is_view_source_ = entry->IsViewSourceMode();
    bindings_ = entry->bindings();
  }

  UpdateLoadFlagsWithCacheFlags(&begin_params_->load_flags,
                                common_params_.navigation_type,
                                common_params_.method == "POST");

  if (entry)
    nav_entry_id_ = entry->GetUniqueID();

  std::string user_agent_override;
  if (request_params.is_overriding_user_agent ||
      (entry && entry->GetIsOverridingUserAgent())) {
    user_agent_override =
        frame_tree_node_->navigator()->GetDelegate()->GetUserAgentOverride();
  }

  std::unique_ptr<net::HttpRequestHeaders> embedder_additional_headers;
  int additional_load_flags = 0;
  GetContentClient()->browser()->NavigationRequestStarted(
      frame_tree_node->frame_tree_node_id(), common_params_.url,
      &embedder_additional_headers, &additional_load_flags);
  begin_params_->load_flags |= additional_load_flags;

  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(begin_params_->headers);
  AddAdditionalRequestHeaders(
      &headers, std::move(embedder_additional_headers), common_params_.url,
      common_params_.navigation_type,
      frame_tree_node_->navigator()->GetController()->GetBrowserContext(),
      common_params.method, user_agent_override, frame_tree_node);

  if (begin_params_->is_form_submission) {
    if (browser_initiated && !request_params.post_content_type.empty()) {
      headers.SetHeaderIfMissing(net::HttpRequestHeaders::kContentType,
                                 request_params.post_content_type);
    } else if (!browser_initiated) {
      headers.GetHeader(net::HttpRequestHeaders::kContentType,
                        &request_params_.post_content_type);
    }
  }

  BrowserContext* browser_context =
      frame_tree_node_->navigator()->GetController()->GetBrowserContext();
  RendererPreferences render_prefs = frame_tree_node_->render_manager()
                                         ->current_host()
                                         ->GetDelegate()
                                         ->GetRendererPrefs(browser_context);
  if (render_prefs.enable_do_not_track)
    headers.SetHeader(kDoNotTrackHeader, "1");

  begin_params_->headers = headers.ToString();
}
