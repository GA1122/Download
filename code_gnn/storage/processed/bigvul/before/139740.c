void ResourceFetcher::ReloadLoFiImages() {
  for (const auto& document_resource : document_resources_) {
    Resource* resource = document_resource.value.Get();
    if (resource)
      resource->ReloadIfLoFiOrPlaceholderImage(this, Resource::kReloadAlways);
  }
}
