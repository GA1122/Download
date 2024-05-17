bool Resource::MustRevalidateDueToCacheHeaders() const {
  return !CanUseResponse(GetResponse(), response_timestamp_) ||
         GetResourceRequest().CacheControlContainsNoCache() ||
         GetResourceRequest().CacheControlContainsNoStore();
}
