bool ShouldHandle(const HttpRequest& request, const std::string& path_prefix) {
  GURL url = request.GetURL();
  return url.path() == path_prefix ||
         base::StartsWith(url.path(), path_prefix + "/",
                          base::CompareCase::SENSITIVE);
}
