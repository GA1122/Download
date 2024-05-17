bool ImageResource::ShouldShowPlaceholder() const {
  if (RuntimeEnabledFeatures::ClientPlaceholdersForServerLoFiEnabled() &&
      (GetResourceRequest().GetPreviewsState() &
       WebURLRequest::kServerLoFiOn)) {
    return true;
  }

  switch (placeholder_option_) {
    case PlaceholderOption::kShowAndReloadPlaceholderAlways:
    case PlaceholderOption::kShowAndDoNotReloadPlaceholder:
      return true;
    case PlaceholderOption::kReloadPlaceholderOnDecodeError:
    case PlaceholderOption::kDoNotReloadPlaceholder:
      return false;
  }
  NOTREACHED();
  return false;
}
