Resource* CachedResource(LocalFrame* frame,
                         const KURL& url,
                         InspectorResourceContentLoader* loader) {
  Document* document = frame->GetDocument();
  if (!document)
    return nullptr;
  Resource* cached_resource = document->Fetcher()->CachedResource(url);
  if (!cached_resource) {
    HeapVector<Member<Document>> all_imports =
        InspectorPageAgent::ImportsForFrame(frame);
    for (Document* import : all_imports) {
      cached_resource = import->Fetcher()->CachedResource(url);
      if (cached_resource)
        break;
    }
  }
  if (!cached_resource) {
    cached_resource = GetMemoryCache()->ResourceForURL(
        url, document->Fetcher()->GetCacheIdentifier());
  }
  if (!cached_resource)
    cached_resource = loader->ResourceForURL(url);
  return cached_resource;
}
