void RenderFrameImpl::DidFailProvisionalLoad(
    const WebURLError& error,
    blink::WebHistoryCommitType commit_type) {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::didFailProvisionalLoad", "id", routing_id_);
  NotifyObserversOfFailedProvisionalLoad(error);

  WebDocumentLoader* document_loader = frame_->GetProvisionalDocumentLoader();
  if (!document_loader)
    return;

  SendFailedProvisionalLoad(document_loader->GetRequest(), error, frame_);

  if (!ShouldDisplayErrorPageForFailedLoad(error.reason(), error.url()))
    return;

  bool replace_current_item = commit_type != blink::kWebStandardCommit;

  bool inherit_document_state =
      !NavigationState::FromDocumentLoader(document_loader)
           ->IsContentInitiated();
  LoadNavigationErrorPage(document_loader, error, base::nullopt,
                          replace_current_item, inherit_document_state);
}
