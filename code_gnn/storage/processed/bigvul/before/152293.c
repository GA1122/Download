void RenderFrameImpl::DidStartProvisionalLoad(
    blink::WebDocumentLoader* document_loader) {
  if (!document_loader)
    return;

  TRACE_EVENT2("navigation,benchmark,rail",
               "RenderFrameImpl::didStartProvisionalLoad", "id", routing_id_,
               "url", document_loader->GetUrl().GetString().Utf8());

  NavigationState* navigation_state =
      NavigationState::FromDocumentLoader(document_loader);
  if (!navigation_state->was_initiated_in_this_frame()) {
    for (auto& observer : observers_)
      observer.DidStartNavigation(document_loader->GetUrl(), base::nullopt);
  }

  for (auto& observer : observers_)
    observer.ReadyToCommitNavigation(document_loader);
}
