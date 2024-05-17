bool Resource::CanUseCacheValidator() const {
  if (IsLoading() || ErrorOccurred())
    return false;

  if (HasCacheControlNoStoreHeader())
    return false;

  if (!RedirectChain().IsEmpty())
    return false;

  return GetResponse().HasCacheValidatorFields() ||
         GetResourceRequest().HasCacheValidatorFields();
}
