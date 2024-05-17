bool ResourceFetcher::ResourceNeedsLoad(Resource* resource,
                                        const FetchParameters& params,
                                        RevalidationPolicy policy) {
  if (resource->GetType() == Resource::kFont && !params.IsLinkPreload())
    return false;

  if (resource->GetType() == Resource::kImage &&
      ShouldDeferImageLoad(resource->Url()))
    return false;

  return policy != kUse || resource->StillNeedsLoad();
}
