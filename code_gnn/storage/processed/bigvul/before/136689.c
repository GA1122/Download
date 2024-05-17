bool FrameLoader::ShouldTreatURLAsSameAsCurrent(const KURL& url) const {
  return document_loader_->GetHistoryItem() &&
         url == document_loader_->GetHistoryItem()->Url();
}
