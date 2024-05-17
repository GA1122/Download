bool FrameFetchContext::ShouldLoadNewResource(Resource::Type type) const {
  if (!document_loader_)
    return true;

  if (IsDetached())
    return false;

  FrameLoader& loader = document_loader_->GetFrame()->Loader();
  if (type == Resource::kMainResource)
    return document_loader_ == loader.GetProvisionalDocumentLoader();
  return document_loader_ == loader.GetDocumentLoader();
}
