bool ResourceFetcher::IsImageResourceDisallowedToBeReused(
    const Resource& existing_resource) const {

  if (existing_resource.GetType() != Resource::kImage)
    return false;

  return !Context().AllowImage(images_enabled_, existing_resource.Url());
}
