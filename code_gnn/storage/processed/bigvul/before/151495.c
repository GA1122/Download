 void WorkerFetchContext::DispatchDidReceiveResponse(
    unsigned long identifier,
    const ResourceResponse& response,
    WebURLRequest::FrameType frame_type,
    WebURLRequest::RequestContext request_context,
    Resource* resource,
    ResourceResponseType) {
  if (response.HasMajorCertificateErrors()) {
    WebMixedContentContextType context_type =
        WebMixedContent::ContextTypeFromRequestContext(
            request_context, false  );
    if (context_type == WebMixedContentContextType::kBlockable) {
      web_context_->DidRunContentWithCertificateErrors(response.Url());
    } else {
      web_context_->DidDisplayContentWithCertificateErrors(response.Url());
    }
  }
  probe::didReceiveResourceResponse(global_scope_, identifier, nullptr,
                                    response, resource);
}
