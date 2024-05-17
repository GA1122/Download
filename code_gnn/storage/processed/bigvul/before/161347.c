Response NetworkHandler::SetCacheDisabled(bool cache_disabled) {
  cache_disabled_ = cache_disabled;
  return Response::FallThrough();
}
