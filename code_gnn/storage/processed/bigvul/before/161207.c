static bool IsUnknownMimeType(const std::string& mime_type) {
  static const char* const kUnknownMimeTypes[] = {
    "",
    "unknown/unknown",
    "application/unknown",
    "*/*",
  };
  for (size_t i = 0; i < arraysize(kUnknownMimeTypes); ++i) {
    if (mime_type == kUnknownMimeTypes[i])
      return true;
  }
  if (mime_type.find('/') == std::string::npos) {
    return true;
  }
  return false;
}
