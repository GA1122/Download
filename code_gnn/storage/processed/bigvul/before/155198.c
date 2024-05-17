void NavigatorImpl::RequestNavigation(FrameTreeNode* frame_tree_node,
                                      const GURL& dest_url,
                                      const Referrer& dest_referrer,
                                      const FrameNavigationEntry& frame_entry,
                                      const NavigationEntryImpl& entry,
                                      ReloadType reload_type,
                                      LoFiState lofi_state,
                                      bool is_same_document_history_load,
                                      bool is_history_navigation_in_new_child,
                                      base::TimeTicks navigation_start) {
  CHECK(IsBrowserSideNavigationEnabled());
  DCHECK(frame_tree_node);

  bool should_dispatch_beforeunload =
      frame_tree_node->current_frame_host()->ShouldDispatchBeforeUnload();
  FrameMsg_Navigate_Type::Value navigation_type =
      GetNavigationType(controller_->GetBrowserContext(), entry, reload_type);
  std::unique_ptr<NavigationRequest> scoped_request =
      NavigationRequest::CreateBrowserInitiated(
          frame_tree_node, dest_url, dest_referrer, frame_entry, entry,
          navigation_type, lofi_state, is_same_document_history_load,
          is_history_navigation_in_new_child, navigation_start, controller_);
  NavigationRequest* navigation_request = scoped_request.get();

  if (dest_url.SchemeIs(url::kJavaScriptScheme)) {
    RenderFrameHostImpl* render_frame_host =
        frame_tree_node->render_manager()->GetFrameHostForNavigation(
            *navigation_request);
    render_frame_host->CommitNavigation(nullptr,   
                                        nullptr,   
                                        navigation_request->common_params(),
                                        navigation_request->request_params(),
                                        navigation_request->is_view_source());
    return;
  }

  frame_tree_node->CreatedNavigationRequest(std::move(scoped_request));
  navigation_request->CreateNavigationHandle(entry.GetUniqueID());

  if (should_dispatch_beforeunload && !IsRendererDebugURL(dest_url)) {
    navigation_request->SetWaitingForRendererResponse();
    frame_tree_node->current_frame_host()->DispatchBeforeUnload(
        true, reload_type != ReloadType::NONE);
  } else {
    navigation_request->BeginNavigation();
  }
}
