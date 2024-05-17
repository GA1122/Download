bool NavigatorImpl::NavigateToEntry(
    FrameTreeNode* frame_tree_node,
    const FrameNavigationEntry& frame_entry,
    const NavigationEntryImpl& entry,
    ReloadType reload_type,
    bool is_same_document_history_load,
    bool is_history_navigation_in_new_child,
    bool is_pending_entry,
    const scoped_refptr<ResourceRequestBodyImpl>& post_body) {
  TRACE_EVENT0("browser,navigation", "NavigatorImpl::NavigateToEntry");

  GURL dest_url = frame_entry.url();
  Referrer dest_referrer = frame_entry.referrer();
  if (reload_type == ReloadType::ORIGINAL_REQUEST_URL &&
      entry.GetOriginalRequestURL().is_valid() && !entry.GetHasPostData()) {
    dest_url = entry.GetOriginalRequestURL();
    dest_referrer = Referrer();
  }

  if (frame_tree_node->IsMainFrame()) {
    const GURL& virtual_url = entry.GetVirtualURL();
    if (!virtual_url.is_valid() && !virtual_url.is_empty()) {
      LOG(WARNING) << "Refusing to load for invalid virtual URL: "
                   << virtual_url.possibly_invalid_spec();
      return false;
    }
  }

  if (!dest_url.is_valid() && !dest_url.is_empty()) {
    LOG(WARNING) << "Refusing to load invalid URL: "
                 << dest_url.possibly_invalid_spec();
    return false;
  }

  if (dest_url.spec().size() > url::kMaxURLChars) {
    LOG(WARNING) << "Refusing to load URL as it exceeds " << url::kMaxURLChars
                 << " characters.";
    return false;
  }

  base::TimeTicks navigation_start = base::TimeTicks::Now();
  TRACE_EVENT_INSTANT_WITH_TIMESTAMP0(
      "navigation,rail", "NavigationTiming navigationStart",
      TRACE_EVENT_SCOPE_GLOBAL, navigation_start);

  PreviewsState previews_state = PREVIEWS_UNSPECIFIED;
  if (!frame_tree_node->IsMainFrame()) {
    previews_state = frame_tree_node->frame_tree()
                     ->root()
                     ->current_frame_host()
                     ->last_navigation_previews_state();
  } else if (reload_type == ReloadType::DISABLE_LOFI_MODE) {
    previews_state = PREVIEWS_NO_TRANSFORM;
  }

  if (IsBrowserSideNavigationEnabled()) {
    navigation_data_.reset(new NavigationMetricsData(navigation_start, dest_url,
                                                     entry.restore_type()));
    RequestNavigation(frame_tree_node, dest_url, dest_referrer, frame_entry,
                      entry, reload_type, previews_state,
                      is_same_document_history_load,
                      is_history_navigation_in_new_child, navigation_start);
    if (frame_tree_node->IsMainFrame() &&
        frame_tree_node->navigation_request()) {
      TRACE_EVENT_ASYNC_BEGIN_WITH_TIMESTAMP1(
          "navigation", "Navigation timeToNetworkStack",
          frame_tree_node->navigation_request()->navigation_handle(),
          navigation_start,
          "FrameTreeNode id", frame_tree_node->frame_tree_node_id());
    }

  } else {
    RenderFrameHostImpl* dest_render_frame_host =
        frame_tree_node->render_manager()->Navigate(
            dest_url, frame_entry, entry, reload_type != ReloadType::NONE);
    if (!dest_render_frame_host)
      return false;   

    if (is_pending_entry)
      CHECK_EQ(controller_->GetPendingEntry(), &entry);

    CheckWebUIRendererDoesNotDisplayNormalURL(dest_render_frame_host, dest_url);

    bool is_transfer = entry.transferred_global_request_id().child_id != -1;
    if (is_transfer)
      dest_render_frame_host->set_is_loading(true);

    if (is_pending_entry && controller_->GetPendingEntryIndex() != -1)
      DCHECK(frame_entry.page_state().IsValid());

    bool is_transfer_to_same =
        is_transfer &&
        entry.transferred_global_request_id().child_id ==
            dest_render_frame_host->GetProcess()->GetID();
    if (!is_transfer_to_same) {
      navigation_data_.reset(new NavigationMetricsData(
          navigation_start, dest_url, entry.restore_type()));
      FrameMsg_Navigate_Type::Value navigation_type = GetNavigationType(
          frame_tree_node->current_url(),   
          dest_url,                         
          reload_type,                      
          entry,                            
          frame_entry,                      
          is_same_document_history_load);   

      dest_render_frame_host->Navigate(
          entry.ConstructCommonNavigationParams(
              frame_entry, post_body, dest_url, dest_referrer, navigation_type,
              previews_state, navigation_start),
          entry.ConstructStartNavigationParams(),
          entry.ConstructRequestNavigationParams(
              frame_entry, GURL(), std::string(),
              is_history_navigation_in_new_child,
              entry.GetSubframeUniqueNames(frame_tree_node),
              frame_tree_node->has_committed_real_load(),
              controller_->GetPendingEntryIndex() == -1,
              controller_->GetIndexOfEntry(&entry),
              controller_->GetLastCommittedEntryIndex(),
              controller_->GetEntryCount()));
    } else {
      dest_render_frame_host->navigation_handle()->set_is_transferring(false);
    }
  }

  if (is_pending_entry)
    CHECK_EQ(controller_->GetPendingEntry(), &entry);

  if (controller_->GetPendingEntryIndex() == -1 &&
      dest_url.SchemeIs(url::kJavaScriptScheme)) {
    return false;
  }

  if (delegate_ && is_pending_entry)
    delegate_->DidStartNavigationToPendingEntry(dest_url, reload_type);

  return true;
}
