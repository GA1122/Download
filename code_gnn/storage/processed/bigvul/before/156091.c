std::string GetContentType(const base::FilePath& path) {
  if (path.MatchesExtension(FILE_PATH_LITERAL(".crx")))
    return "application/x-chrome-extension";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".css")))
    return "text/css";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".exe")))
    return "application/octet-stream";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".gif")))
    return "image/gif";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".gzip")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".gz"))) {
    return "application/x-gzip";
  }
  if (path.MatchesExtension(FILE_PATH_LITERAL(".jpeg")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".jpg"))) {
    return "image/jpeg";
  }
  if (path.MatchesExtension(FILE_PATH_LITERAL(".js")))
    return "application/javascript";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".json")))
    return "application/json";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".pdf")))
    return "application/pdf";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".txt")))
    return "text/plain";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".wav")))
    return "audio/wav";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".xml")))
    return "text/xml";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".mhtml")))
    return "multipart/related";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".mht")))
    return "message/rfc822";
  if (path.MatchesExtension(FILE_PATH_LITERAL(".html")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".htm"))) {
    return "text/html";
  }
  return "";
}
