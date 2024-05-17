void ResourceFetcher::ReloadImagesIfNotDeferred() {
  for (Resource* resource : document_resources_) {
    if (resource->GetType() == Resource::kImage && resource->StillNeedsLoad() &&
        !ShouldDeferImageLoad(resource->Url()))
      StartLoad(resource);
  }
}
