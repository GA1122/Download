    LRUCanvasResourceProviderCache::GetCanvasResourceProvider(
        const IntSize& size) {
  wtf_size_t i;
  for (i = 0; i < resource_providers_.size(); ++i) {
    CanvasResourceProvider* resource_provider = resource_providers_[i].get();
    if (!resource_provider)
      break;
    if (resource_provider->Size() != size)
      continue;
    BubbleToFront(i);
    return resource_provider;
  }

  std::unique_ptr<CanvasResourceProvider> temp(CanvasResourceProvider::Create(
      size, CanvasResourceProvider::kSoftwareResourceUsage,
      nullptr,               
      0,                     
      CanvasColorParams(),   
      CanvasResourceProvider::kDefaultPresentationMode,
      nullptr));   
  if (!temp)
    return nullptr;
  i = std::min(resource_providers_.size() - 1, i);
  resource_providers_[i] = std::move(temp);

  CanvasResourceProvider* resource_provider = resource_providers_[i].get();
  BubbleToFront(i);
  return resource_provider;
}
