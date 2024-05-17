static bool CanUseResponse(const ResourceResponse& response,
                           double response_timestamp) {
  if (response.IsNull())
    return false;

  if (response.CacheControlContainsNoCache() ||
      response.CacheControlContainsNoStore())
    return false;

  if (response.HttpStatusCode() == 303) {
    return false;
  }

  if (response.HttpStatusCode() == 302 || response.HttpStatusCode() == 307) {
    bool has_max_age = std::isfinite(response.CacheControlMaxAge());
    bool has_expires = std::isfinite(response.Expires());
    if (!has_max_age && !has_expires)
      return false;
  }

  return CurrentAge(response, response_timestamp) <=
         FreshnessLifetime(response, response_timestamp);
}
