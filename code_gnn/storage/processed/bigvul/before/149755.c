bool CanStripTrailingSlash(const GURL& url) {
  return url.IsStandard() && !url.SchemeIsFile() && !url.SchemeIsFileSystem() &&
         !url.has_query() && !url.has_ref() && url.path_piece() == "/";
}
