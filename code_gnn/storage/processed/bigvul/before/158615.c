void FrameFetchContext::DispatchWillSendRequest(
    unsigned long identifier,
    ResourceRequest& request,
    const ResourceResponse& redirect_response,
    Resource::Type resource_type,
    const FetchInitiatorInfo& initiator_info) {
  if (IsDetached())
    return;

  if (redirect_response.IsNull()) {
    GetFrame()->Loader().Progress().WillStartLoading(identifier,
                                                     request.Priority());
  }
  probe::willSendRequest(GetFrame()->GetDocument(), identifier,
                         MasterDocumentLoader(), request, redirect_response,
                         initiator_info, resource_type);
  if (IdlenessDetector* idleness_detector = GetFrame()->GetIdlenessDetector())
    idleness_detector->OnWillSendRequest(MasterDocumentLoader()->Fetcher());
  if (document_) {
    InteractiveDetector* interactive_detector(
        InteractiveDetector::From(*document_));
    if (interactive_detector) {
      interactive_detector->OnResourceLoadBegin(base::nullopt);
    }
  }
}
