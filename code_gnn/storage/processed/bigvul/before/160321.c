bool ImageLoader::ShouldLoadImmediately(const KURL& url) const {
  if (!url.IsNull()) {
    Resource* resource = GetMemoryCache()->ResourceForURL(
        url, element_->GetDocument().Fetcher()->GetCacheIdentifier());
    if (resource && !resource->ErrorOccurred())
      return true;
  }
  return (IsHTMLObjectElement(element_) || IsHTMLEmbedElement(element_));
}
