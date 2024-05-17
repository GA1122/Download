Document& Document::AXObjectCacheOwner() const {
  Document* doc = const_cast<Document*>(this);
  if (doc->GetFrame() && doc->GetFrame()->PagePopupOwner()) {
    DCHECK(!doc->ax_object_cache_);
    return doc->GetFrame()
        ->PagePopupOwner()
        ->GetDocument()
        .AXObjectCacheOwner();
  }
  return *doc;
}
