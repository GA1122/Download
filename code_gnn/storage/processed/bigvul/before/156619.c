void NavigatorImpl::DidNavigate(
    RenderFrameHostImpl* render_frame_host,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    std::unique_ptr<NavigationHandleImpl> navigation_handle,
    bool was_within_same_document) {
  FrameTreeNode* frame_tree_node = render_frame_host->frame_tree_node();
  FrameTree* frame_tree = frame_tree_node->frame_tree();

  bool is_same_document_navigation = controller_->IsURLSameDocumentNavigation(
      params.url, params.origin, was_within_same_document, render_frame_host);

  if (is_same_document_navigation &&
      render_frame_host !=
          frame_tree_node->render_manager()->current_frame_host()) {
    bad_message::ReceivedBadMessage(render_frame_host->GetProcess(),
                                    bad_message::NI_IN_PAGE_NAVIGATION);
    is_same_document_navigation = false;
  }

  if (ui::PageTransitionIsMainFrame(params.transition)) {
    if (delegate_) {
      if (delegate_->CanOverscrollContent()) {
        if (!was_within_same_document)
          controller_->TakeScreenshot();
      }

      delegate_->DidNavigateMainFramePreCommit(is_same_document_navigation);
    }
  }

  frame_tree_node->render_manager()->DidNavigateFrame(
      render_frame_host, params.gesture == NavigationGestureUser,
      is_same_document_navigation);

  frame_tree_node->SetCurrentOrigin(
      params.origin, params.has_potentially_trustworthy_unique_origin);
  frame_tree_node->SetInsecureRequestPolicy(params.insecure_request_policy);
  frame_tree_node->SetInsecureNavigationsSet(params.insecure_navigations_set);

  if (!is_same_document_navigation) {
    render_frame_host->ResetContentSecurityPolicies();
    frame_tree_node->ResetForNavigation();
  }

  SiteInstanceImpl* site_instance = render_frame_host->GetSiteInstance();
  if (!site_instance->HasSite() &&
      SiteInstanceImpl::ShouldAssignSiteForURL(params.url)) {
    site_instance->SetSite(params.url);
  }

  if (ui::PageTransitionIsMainFrame(params.transition) && delegate_)
    delegate_->SetMainFrameMimeType(params.contents_mime_type);

  int old_entry_count = controller_->GetEntryCount();
  LoadCommittedDetails details;
  bool did_navigate = controller_->RendererDidNavigate(
      render_frame_host, params, &details, is_same_document_navigation,
      navigation_handle.get());

  if (old_entry_count != controller_->GetEntryCount() ||
      details.previous_entry_index !=
          controller_->GetLastCommittedEntryIndex()) {
    frame_tree->root()->render_manager()->SendPageMessage(
        new PageMsg_SetHistoryOffsetAndLength(
            MSG_ROUTING_NONE, controller_->GetLastCommittedEntryIndex(),
            controller_->GetEntryCount()),
        site_instance);
  }

  render_frame_host->DidNavigate(params, is_same_document_navigation);

  if (details.type != NAVIGATION_TYPE_NAV_IGNORE && delegate_) {
    DCHECK_EQ(!render_frame_host->GetParent(),
              did_navigate ? details.is_main_frame : false);
    navigation_handle->DidCommitNavigation(
        params, did_navigate, details.did_replace_entry, details.previous_url,
        details.type, render_frame_host);
    navigation_handle.reset();
  }

  if (!did_navigate)
    return;   


  RecordNavigationMetrics(details, params, site_instance);

  if (delegate_) {
    if (details.is_main_frame) {
      delegate_->DidNavigateMainFramePostCommit(render_frame_host, details,
                                                params);
    }

    delegate_->DidNavigateAnyFramePostCommit(render_frame_host, details,
                                             params);
  }
}
