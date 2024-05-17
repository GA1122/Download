  std::string GetUrlPath(const std::string& url) const {
    GURL gurl(url);
    return gurl.path();
  }
