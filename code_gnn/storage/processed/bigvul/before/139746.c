bool ResourceFetcher::ResourceNeedsLoad(Resource* resource,
                                        const FetchRequest& request,
                                        RevalidationPolicy policy) {
  if (resource->GetType() == Resource::kFont && !request.IsLinkPreload())
    return false;
  if (resource->IsImage() && ShouldDeferImageLoad(resource->Url()))
    return false;
  return policy != kUse || resource->StillNeedsLoad();
}
