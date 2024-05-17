void FrameFetchContext::DidLoadResource(Resource* resource) {
  if (!document_)
    return;
  FirstMeaningfulPaintDetector::From(*document_).CheckNetworkStable();
  if (LocalFrame* local_frame = document_->GetFrame()) {
    if (IdlenessDetector* idleness_detector =
            local_frame->GetIdlenessDetector()) {
      idleness_detector->OnDidLoadResource();
    }
  }

  if (resource->IsLoadEventBlockingResourceType())
    document_->CheckCompleted();
}
