ImageResource* ImageResource::Create(const ResourceRequest& request) {
  ResourceLoaderOptions options;
  return new ImageResource(request, options,
                           ImageResourceContent::CreateNotStarted(), false);
}
