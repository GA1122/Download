void RenderFrameImpl::WillSendRequest(blink::WebURLRequest& request) {
  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
  WillSendRequestInternal(
      request, WebURLRequestToResourceType(request),
      DocumentState::FromDocumentLoader(document_loader),
      GetTransitionType(document_loader, IsMainFrame(), false  ));
}
