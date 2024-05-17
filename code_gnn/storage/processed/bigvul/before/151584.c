bool ShouldResourceBeAddedToMemoryCache(const FetchParameters& params,
                                        Resource* resource) {
  if (!IsMainThread())
    return false;
  if (params.Options().data_buffering_policy == kDoNotBufferData)
    return false;
  if (IsRawResource(*resource))
    return false;
  return true;
}
