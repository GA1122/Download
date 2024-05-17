bool RenderFrameHostImpl::DidCommitNavigationInternal(
    std::unique_ptr<NavigationRequest> navigation_request,
    FrameHostMsg_DidCommitProvisionalLoad_Params* validated_params,
    bool is_same_document_navigation) {
  DCHECK_EQ(ui::PageTransitionIsMainFrame(validated_params->transition),
            !GetParent());

  std::unique_ptr<NavigationRequest> invalid_request = nullptr;
  if (navigation_request &&
      navigation_request->commit_params().navigation_token !=
          validated_params->navigation_token) {
    navigation_request.reset();
  }

  if (!ValidateDidCommitParams(navigation_request.get(), validated_params,
                               is_same_document_navigation)) {
    return false;
  }

  if (navigation_request &&
      navigation_request->common_params().url != validated_params->url) {

    invalid_request = std::move(navigation_request);
  }

  if (!is_loading()) {
    bool was_loading = frame_tree_node()->frame_tree()->IsLoading();
    is_loading_ = true;
    frame_tree_node()->DidStartLoading(!is_same_document_navigation,
                                       was_loading);
  }

  if (navigation_request)
    was_discarded_ = navigation_request->commit_params().was_discarded;

  if (!navigation_request) {
    NavigationEntryImpl* entry_for_navigation = nullptr;
    if (invalid_request && NavigationRequestWasIntendedForPendingEntry(
                               invalid_request.get(), *validated_params,
                               is_same_document_navigation)) {
      entry_for_navigation = NavigationEntryImpl::FromNavigationEntry(
          frame_tree_node()->navigator()->GetController()->GetPendingEntry());
    }

    navigation_request = CreateNavigationRequestForCommit(
        *validated_params, is_same_document_navigation, entry_for_navigation);
  }

  DCHECK(navigation_request);
  DCHECK(navigation_request->navigation_handle());

  navigation_request->set_transition(validated_params->transition);

  navigation_request->set_has_user_gesture(validated_params->gesture ==
                                           NavigationGestureUser);

  UpdateSiteURL(validated_params->url, validated_params->url_is_unreachable);

  is_mhtml_document_ =
      (navigation_request->GetMimeType() == "multipart/related" ||
       navigation_request->GetMimeType() == "message/rfc822");

  accessibility_reset_count_ = 0;
  appcache_handle_ =
      navigation_request->navigation_handle()->TakeAppCacheHandle();
  frame_tree_node()->navigator()->DidNavigate(this, *validated_params,
                                              std::move(navigation_request),
                                              is_same_document_navigation);

  if (is_same_document_navigation && invalid_request)
    same_document_navigation_request_ = std::move(invalid_request);

  if (!is_same_document_navigation)
    scheduler_tracked_features_ = 0;

  return true;
}
