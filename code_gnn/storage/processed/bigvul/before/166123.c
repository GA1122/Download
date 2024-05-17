bool RenderFrameHostImpl::DidCommitNavigationInternal(
    FrameHostMsg_DidCommitProvisionalLoad_Params* validated_params,
    bool is_same_document_navigation) {
  DCHECK_EQ(ui::PageTransitionIsMainFrame(validated_params->transition),
            !GetParent());

  if (!ValidateDidCommitParams(validated_params))
    return false;

  if (!is_loading()) {
    bool was_loading = frame_tree_node()->frame_tree()->IsLoading();
    is_loading_ = true;
    frame_tree_node()->DidStartLoading(!is_same_document_navigation,
                                       was_loading);
  }

  if (navigation_request_)
    was_discarded_ = navigation_request_->request_params().was_discarded;

  std::unique_ptr<NavigationRequest> navigation_request;

  if (is_same_document_navigation) {
    navigation_request =
        TakeNavigationRequestForSameDocumentCommit(*validated_params);
  } else {
    navigation_request = TakeNavigationRequestForCommit(*validated_params);
  }
  DCHECK(navigation_request);
  DCHECK(navigation_request->navigation_handle());

  UpdateSiteURL(validated_params->url, validated_params->url_is_unreachable);

  accessibility_reset_count_ = 0;
  frame_tree_node()->navigator()->DidNavigate(this, *validated_params,
                                              std::move(navigation_request),
                                              is_same_document_navigation);
  if (!is_same_document_navigation)
    navigation_request_.reset();
  return true;
}
