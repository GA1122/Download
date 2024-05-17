bool ImageResource::CanReuse(const FetchParameters& params) const {
  if (params.GetPlaceholderImageRequestType() !=
          FetchParameters::kAllowPlaceholder &&
      placeholder_option_ != PlaceholderOption::kDoNotReloadPlaceholder)
    return false;

  return Resource::CanReuse(params);
}
