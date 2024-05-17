GURL GetWebUIURL(std::string host) {
  return GURL(std::string(kChromeUIScheme) + "://" + host);
}
