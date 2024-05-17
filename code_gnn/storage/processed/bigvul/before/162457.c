ImageResource* ImageResource::CreateForTest(const KURL& url) {
  ResourceRequest request(url);
  return Create(request);
}
