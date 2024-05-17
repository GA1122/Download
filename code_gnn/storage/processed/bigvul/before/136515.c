bool PaintController::ClientCacheIsValid(
    const DisplayItemClient& client) const {
#if DCHECK_IS_ON()
  DCHECK(client.IsAlive());
#endif
  if (IsSkippingCache())
    return false;
  return client.DisplayItemsAreCached(current_cache_generation_);
}
