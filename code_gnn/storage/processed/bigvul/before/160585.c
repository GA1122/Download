void RenderFrameImpl::DidFailProvisionalLoadInternal(
    const WebURLError& error,
    blink::WebHistoryCommitType commit_type,
    const base::Optional<std::string>& error_page_content) {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::didFailProvisionalLoad", "id", routing_id_);
  for (auto& observer : render_view_->observers())
    observer.DidFailProvisionalLoad(frame_, error);
  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.DidFailProvisionalLoad");
    for (auto& observer : observers_)
      observer.DidFailProvisionalLoad(error);
  }

  WebDocumentLoader* document_loader = frame_->GetProvisionalDocumentLoader();
  if (!document_loader)
    return;

  const WebURLRequest& failed_request = document_loader->GetRequest();

  SendFailedProvisionalLoad(failed_request, error, frame_);

  if (!ShouldDisplayErrorPageForFailedLoad(error.reason(), error.url()))
    return;

  frame_->EnableViewSourceMode(false);

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(document_loader);
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());

  bool replace = commit_type != blink::kWebStandardCommit;

  if (!navigation_state->IsContentInitiated()) {
    pending_navigation_params_.reset(new NavigationParams(
        navigation_state->common_params(), navigation_state->request_params()));
  }

  LoadNavigationErrorPage(failed_request, error, replace, nullptr,
                          error_page_content);
}
