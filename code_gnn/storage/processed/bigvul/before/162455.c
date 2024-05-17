bool ImageResource::CanUseCacheValidator() const {
  if (!GetContent()->IsLoaded())
    return false;

  return Resource::CanUseCacheValidator();
}
