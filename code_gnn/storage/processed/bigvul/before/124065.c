GURL AddUberHost(const GURL& url) {
  const std::string uber_host = chrome::kChromeUIUberHost;
  const std::string new_path = url.host() + url.path();

  return ReplaceURLHostAndPath(url, uber_host, new_path);
}
