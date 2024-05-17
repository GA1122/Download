void NavigatorImpl::RequestNavigation(FrameTreeNode* frame_tree_node,
                                      const GURL& dest_url,
                                      const Referrer& dest_referrer,
                                      const FrameNavigationEntry& frame_entry,
                                      const NavigationEntryImpl& entry,
                                      ReloadType reload_type,
                                      PreviewsState previews_state,
                                      bool is_same_document_history_load,
                                      bool is_history_navigation_in_new_child,
                                      base::TimeTicks navigation_start) {
  CHECK(IsBrowserSideNavigationEnabled());
  DCHECK(frame_tree_node);

  bool should_dispatch_beforeunload =
      !is_same_document_history_load &&
      !is_history_navigation_in_new_child &&
      frame_tree_node->current_frame_host()->ShouldDispatchBeforeUnload();
  FrameMsg_Navigate_Type::Value navigation_type = GetNavigationType(
      frame_tree_node->current_url(),   
      dest_url,                         
      reload_type,                      
      entry,                            
      frame_entry,                      
      is_same_document_history_load);   
  std::unique_ptr<NavigationRequest> scoped_request =
      NavigationRequest::CreateBrowserInitiated(
          frame_tree_node, dest_url, dest_referrer, frame_entry, entry,
          navigation_type, previews_state, is_same_document_history_load,
          is_history_navigation_in_new_child, navigation_start, controller_);

  if (dest_url.SchemeIs(url::kJavaScriptScheme)) {
    RenderFrameHostImpl* render_frame_host =
        frame_tree_node->render_manager()->GetFrameHostForNavigation(
            *scoped_request.get());
    render_frame_host->CommitNavigation(
        nullptr,   
        nullptr,   
        mojo::ScopedDataPipeConsumerHandle(), scoped_request->common_params(),
        scoped_request->request_params(), scoped_request->is_view_source());
    return;
  }

  frame_tree_node->CreatedNavigationRequest(std::move(scoped_request));

  frame_tree_node->navigation_request()->CreateNavigationHandle(
      entry.GetUniqueID());

  NavigationRequest* navigation_request = frame_tree_node->navigation_request();
  if (!navigation_request)
    return;   

  navigation_request->navigation_handle()->set_base_url_for_data_url(
      entry.GetBaseURLForDataURL());

  if (should_dispatch_beforeunload && !IsRendererDebugURL(dest_url)) {
    navigation_request->SetWaitingForRendererResponse();
    frame_tree_node->current_frame_host()->DispatchBeforeUnload(
        true, reload_type != ReloadType::NONE);
  } else {
    navigation_request->BeginNavigation();
  }
}
