void ResourceFetcher::HandleLoadCompletion(Resource* resource) {
  Context().DidLoadResource(resource);

  resource->ReloadIfLoFiOrPlaceholderImage(this, Resource::kReloadIfNeeded);
}
