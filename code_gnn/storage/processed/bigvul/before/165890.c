void RenderFrameImpl::DidCommitNavigationInternal(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type,
    bool was_within_same_document,
    ui::PageTransition transition,
    service_manager::mojom::InterfaceProviderRequest
        remote_interface_provider_request) {
  DCHECK(!(was_within_same_document &&
           remote_interface_provider_request.is_pending()));
  UpdateStateForCommit(item, commit_type, transition);

  if (was_within_same_document) {
    GetFrameHost()->DidCommitSameDocumentNavigation(
        MakeDidCommitProvisionalLoadParams(commit_type, transition));
  } else {
    GetFrameHost()->DidCommitProvisionalLoad(
        MakeDidCommitProvisionalLoadParams(commit_type, transition),
        std::move(remote_interface_provider_request));
  }
}
