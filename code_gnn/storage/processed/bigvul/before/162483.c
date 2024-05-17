bool ImageResource::ShouldReloadBrokenPlaceholder() const {
  switch (placeholder_option_) {
    case PlaceholderOption::kShowAndReloadPlaceholderAlways:
      return ErrorOccurred();
    case PlaceholderOption::kReloadPlaceholderOnDecodeError:
      return GetStatus() == ResourceStatus::kDecodeError;
    case PlaceholderOption::kShowAndDoNotReloadPlaceholder:
    case PlaceholderOption::kDoNotReloadPlaceholder:
      return false;
  }
  NOTREACHED();
  return false;
}
