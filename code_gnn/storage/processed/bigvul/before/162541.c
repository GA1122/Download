static double FreshnessLifetime(const ResourceResponse& response,
                                double response_timestamp) {
#if !defined(OS_ANDROID)
  if (response.Url().IsLocalFile())
    return 0;
#endif

  if (!response.Url().ProtocolIsInHTTPFamily() &&
      !response.Url().ProtocolIs("filesystem"))
    return std::numeric_limits<double>::max();

  double max_age_value = response.CacheControlMaxAge();
  if (std::isfinite(max_age_value))
    return max_age_value;
  double expires_value = response.Expires();
  double date_value = response.Date();
  double creation_time =
      std::isfinite(date_value) ? date_value : response_timestamp;
  if (std::isfinite(expires_value))
    return expires_value - creation_time;
  double last_modified_value = response.LastModified();
  if (std::isfinite(last_modified_value))
    return (creation_time - last_modified_value) * 0.1;
  return 0;
}
