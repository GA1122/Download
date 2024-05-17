void RenderFrameHostImpl::NavigationRequestCancelled(
    NavigationRequest* navigation_request) {
  OnCrossDocumentCommitProcessed(navigation_request,
                                 blink::mojom::CommitResult::Aborted);
}
