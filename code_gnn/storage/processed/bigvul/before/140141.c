String urlForLoggingMedia(const KURL& url) {
  static const unsigned maximumURLLengthForLogging = 128;

  if (url.getString().length() < maximumURLLengthForLogging)
    return url.getString();
  return url.getString().substring(0, maximumURLLengthForLogging) + "...";
}
