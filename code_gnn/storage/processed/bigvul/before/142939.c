String UrlForLoggingMedia(const KURL& url) {
  static const unsigned kMaximumURLLengthForLogging = 128;

  if (url.GetString().length() < kMaximumURLLengthForLogging)
    return url.GetString();
  return url.GetString().Substring(0, kMaximumURLLengthForLogging) + "...";
}
