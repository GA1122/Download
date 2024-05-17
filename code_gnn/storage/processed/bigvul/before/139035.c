bool ImageLoader::shouldLoadImmediately(const KURL& url) const {
  if (!url.isNull()) {
    Resource* resource = memoryCache()->resourceForURL(
        url, m_element->document().fetcher()->getCacheIdentifier());
    if (resource && !resource->errorOccurred())
      return true;
  }
  return (isHTMLObjectElement(m_element) || isHTMLEmbedElement(m_element));
}
