bool DocumentInit::ShouldTreatURLAsSrcdocDocument() const {
  return parent_document_ &&
         document_loader_->GetFrame()->Loader().ShouldTreatURLAsSrcdocDocument(
             url_);
}
