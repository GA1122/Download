void NavigatorImpl::OnBeginNavigation(
    FrameTreeNode* frame_tree_node,
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory,
    mojom::NavigationClientAssociatedPtrInfo navigation_client) {
  DCHECK(frame_tree_node);

  NavigationRequest* ongoing_navigation_request =
      frame_tree_node->navigation_request();

  if (ongoing_navigation_request && ongoing_navigation_request->request_params()
                                        .is_history_navigation_in_new_child) {
    ongoing_navigation_request = nullptr;
    frame_tree_node->ResetNavigationRequest(false, true);
  }

  if (ongoing_navigation_request &&
      ongoing_navigation_request->browser_initiated() &&
      !common_params.has_user_gesture) {
    if (!IsPerNavigationMojoInterfaceEnabled()) {
      RenderFrameHost* current_frame_host =
          frame_tree_node->render_manager()->current_frame_host();
      current_frame_host->Send(
          new FrameMsg_DroppedNavigation(current_frame_host->GetRoutingID()));
    }
    return;
  }

  if (frame_tree_node->IsMainFrame()) {
    DidStartMainFrameNavigation(
        common_params.url,
        frame_tree_node->current_frame_host()->GetSiteInstance(), nullptr);
    navigation_data_.reset();
  }
  NavigationEntryImpl* pending_entry = controller_->GetPendingEntry();
  NavigationEntryImpl* current_entry = controller_->GetLastCommittedEntry();
  bool override_user_agent =
      current_entry
          ? current_entry->GetIsOverridingUserAgent()
          : delegate_ && delegate_->ShouldOverrideUserAgentInNewTabs();
  frame_tree_node->CreatedNavigationRequest(
      NavigationRequest::CreateRendererInitiated(
          frame_tree_node, pending_entry, common_params,
          std::move(begin_params), controller_->GetLastCommittedEntryIndex(),
          controller_->GetEntryCount(), override_user_agent,
          std::move(blob_url_loader_factory), std::move(navigation_client)));
  NavigationRequest* navigation_request = frame_tree_node->navigation_request();

  DCHECK(!FrameMsg_Navigate_Type::IsSameDocument(
      navigation_request->common_params().navigation_type));
  bool should_dispatch_beforeunload =
      frame_tree_node->current_frame_host()->ShouldDispatchBeforeUnload(
          true  );
  if (should_dispatch_beforeunload) {
    frame_tree_node->navigation_request()->SetWaitingForRendererResponse();
    frame_tree_node->current_frame_host()->DispatchBeforeUnload(
        RenderFrameHostImpl::BeforeUnloadType::RENDERER_INITIATED_NAVIGATION,
        FrameMsg_Navigate_Type::IsReload(common_params.navigation_type));
    return;
  }

  navigation_request->BeginNavigation();
}
