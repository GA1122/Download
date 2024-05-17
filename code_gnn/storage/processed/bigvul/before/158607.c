void FrameFetchContext::DidLoadResource(Resource* resource) {
  if (!document_)
    return;
  if (LocalFrame* local_frame = document_->GetFrame()) {
    if (IdlenessDetector* idleness_detector =
            local_frame->GetIdlenessDetector()) {
      idleness_detector->OnDidLoadResource();
    }
  }

  if (resource->IsLoadEventBlockingResourceType())
    document_->CheckCompleted();
}
