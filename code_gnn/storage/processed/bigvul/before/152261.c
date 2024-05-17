void RenderFrameImpl::DidCommitNavigationInternal(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type,
    bool was_within_same_document,
    ui::PageTransition transition,
    mojom::DidCommitProvisionalLoadInterfaceParamsPtr interface_params) {
  DCHECK(!(was_within_same_document && interface_params));
  UpdateStateForCommit(item, commit_type, transition);

  auto params = MakeDidCommitProvisionalLoadParams(commit_type, transition);
  if (was_within_same_document) {
    GetFrameHost()->DidCommitSameDocumentNavigation(std::move(params));
  } else {
    NavigationState* navigation_state =
        NavigationState::FromDocumentLoader(frame_->GetDocumentLoader());
    if (navigation_state->uses_per_navigation_mojo_interface()) {
      navigation_state->RunPerNavigationInterfaceCommitNavigationCallback(
          std::move(params), std::move(interface_params));
    } else {
      GetFrameHost()->DidCommitProvisionalLoad(std::move(params),
                                               std::move(interface_params));
    }
  }
}
