void ResourceFetcher::ReloadLoFiImages() {
  for (Resource* resource : document_resources_) {
    if (resource)
      resource->ReloadIfLoFiOrPlaceholderImage(this, Resource::kReloadAlways);
  }
}
