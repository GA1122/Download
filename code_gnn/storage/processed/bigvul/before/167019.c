void NavigationControllerImpl::LoadURLWithParams(const LoadURLParams& params) {
  TRACE_EVENT1("browser,navigation",
               "NavigationControllerImpl::LoadURLWithParams",
               "url", params.url.possibly_invalid_spec());
  if (HandleDebugURL(params.url, params.transition_type)) {
    if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
            cc::switches::kEnableGpuBenchmarking))
      return;
  }

  switch (params.load_type) {
    case LOAD_TYPE_DEFAULT:
    case LOAD_TYPE_HTTP_POST:
      break;
    case LOAD_TYPE_DATA:
      if (!params.url.SchemeIs(url::kDataScheme)) {
        NOTREACHED() << "Data load must use data scheme.";
        return;
      }
      break;
    default:
      NOTREACHED();
      break;
  }

  needs_reload_ = false;

  bool override = false;
  switch (params.override_user_agent) {
    case UA_OVERRIDE_INHERIT:
      override = ShouldKeepOverride(GetLastCommittedEntry());
      break;
    case UA_OVERRIDE_TRUE:
      override = true;
      break;
    case UA_OVERRIDE_FALSE:
      override = false;
      break;
    default:
      NOTREACHED();
      break;
  }

  std::unique_ptr<NavigationEntryImpl> entry;

  int frame_tree_node_id = params.frame_tree_node_id;
  DCHECK(frame_tree_node_id == RenderFrameHost::kNoFrameTreeNodeId ||
         !params.navigation_ui_data);

  if (frame_tree_node_id != RenderFrameHost::kNoFrameTreeNodeId ||
      !params.frame_name.empty()) {
    FrameTreeNode* node =
        params.frame_tree_node_id != RenderFrameHost::kNoFrameTreeNodeId
            ? delegate_->GetFrameTree()->FindByID(params.frame_tree_node_id)
            : delegate_->GetFrameTree()->FindByName(params.frame_name);
    if (node && !node->IsMainFrame()) {
      DCHECK(GetLastCommittedEntry());

      frame_tree_node_id = node->frame_tree_node_id();

      entry = GetLastCommittedEntry()->Clone();
      entry->AddOrUpdateFrameEntry(
          node, -1, -1, nullptr,
          static_cast<SiteInstanceImpl*>(params.source_site_instance.get()),
          params.url, params.referrer, params.redirect_chain, PageState(),
          "GET", -1, params.blob_url_loader_factory);
    }
  }

  if (!entry) {
    std::string extra_headers_crlf;
    base::ReplaceChars(params.extra_headers, "\n", "\r\n", &extra_headers_crlf);
    entry = NavigationEntryImpl::FromNavigationEntry(CreateNavigationEntry(
        params.url, params.referrer, params.transition_type,
        params.is_renderer_initiated, extra_headers_crlf, browser_context_,
        params.blob_url_loader_factory));
    entry->set_source_site_instance(
        static_cast<SiteInstanceImpl*>(params.source_site_instance.get()));
    entry->SetRedirectChain(params.redirect_chain);
  }

  entry->set_frame_tree_node_id(frame_tree_node_id);
  if (params.should_replace_current_entry && entries_.size() > 0)
    entry->set_should_replace_entry(true);
  entry->set_should_clear_history_list(params.should_clear_history_list);
  entry->SetIsOverridingUserAgent(override);
  entry->set_transferred_global_request_id(
      params.transferred_global_request_id);

#if defined(OS_ANDROID)
  entry->set_has_user_gesture(params.has_user_gesture);
#else
  if (params.is_renderer_initiated)
    entry->set_has_user_gesture(params.has_user_gesture);
#endif

#if defined(OS_ANDROID)
  if (params.intent_received_timestamp > 0) {
    entry->set_intent_received_timestamp(
        base::TimeTicks() +
        base::TimeDelta::FromMilliseconds(params.intent_received_timestamp));
  }
#endif

  switch (params.load_type) {
    case LOAD_TYPE_DEFAULT:
      break;
    case LOAD_TYPE_HTTP_POST:
      entry->SetHasPostData(true);
      entry->SetPostData(params.post_data);
      break;
    case LOAD_TYPE_DATA:
      entry->SetBaseURLForDataURL(params.base_url_for_data_url);
      entry->SetVirtualURL(params.virtual_url_for_data_url);
#if defined(OS_ANDROID)
      entry->SetDataURLAsString(params.data_url_as_string);
#endif
      entry->SetCanLoadLocalResources(params.can_load_local_resources);
      break;
    default:
      NOTREACHED();
      break;
  }

  entry->set_started_from_context_menu(params.started_from_context_menu);
  LoadEntry(std::move(entry), params.navigation_ui_data
                                  ? params.navigation_ui_data->Clone()
                                  : nullptr);
}
