void RenderFrameImpl::DidFailLoad(const WebURLError& error,
                                  blink::WebHistoryCommitType commit_type) {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::didFailLoad",
               "id", routing_id_);
  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
  DCHECK(document_loader);

  base::string16 error_description;
  GetContentClient()->renderer()->GetErrorDescription(
      error, document_loader->HttpMethod().Ascii(), &error_description);
  Send(new FrameHostMsg_DidFailLoadWithError(
      routing_id_, document_loader->GetUrl(), error.reason(),
      error_description));
}
