void RenderFrameImpl::DidNavigateWithinPage(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type,
    bool content_initiated) {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::didNavigateWithinPage",
               "id", routing_id_);
  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());
  UpdateNavigationState(document_state, true  ,
                        content_initiated);
  static_cast<NavigationStateImpl*>(document_state->navigation_state())
      ->set_was_within_same_document(true);

  DidCommitProvisionalLoad(item, commit_type,
                           blink::WebGlobalObjectReusePolicy::kUseExisting);
}
