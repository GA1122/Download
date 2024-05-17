bool ShouldSniffMimeType(const GURL& url, const std::string& mime_type) {
  bool sniffable_scheme = url.is_empty() ||
                          url.SchemeIsHTTPOrHTTPS() ||
                          url.SchemeIs("ftp") ||
#if defined(OS_ANDROID)
                          url.SchemeIs("content") ||
#endif
                          url.SchemeIsFile() ||
                          url.SchemeIsFileSystem();
  if (!sniffable_scheme)
    return false;

  static const char* const kSniffableTypes[] = {
    "text/plain",
    "application/octet-stream",
    "text/xml",
    "application/xml",
    "application/msword",
    "application/vnd.ms-excel",
    "application/vnd.ms-powerpoint",
    "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
    "application/vnd.openxmlformats-officedocument.presentationml.presentation",
    "application/vnd.ms-excel.sheet.macroenabled.12",
    "application/vnd.ms-word.document.macroenabled.12",
    "application/vnd.ms-powerpoint.presentation.macroenabled.12",
    "application/mspowerpoint",
    "application/msexcel",
    "application/vnd.ms-word",
    "application/vnd.ms-word.document.12",
    "application/vnd.msword",
  };
  for (size_t i = 0; i < arraysize(kSniffableTypes); ++i) {
    if (mime_type == kSniffableTypes[i])
      return true;
  }
  if (IsUnknownMimeType(mime_type)) {
    return true;
  }
  return false;
}
