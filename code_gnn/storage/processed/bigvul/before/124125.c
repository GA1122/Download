GURL ReplaceURLHostAndPath(const GURL& url,
                           const std::string& host,
                           const std::string& path) {
  url_canon::Replacements<char> replacements;
  replacements.SetHost(host.c_str(),
                       url_parse::Component(0, host.length()));
  replacements.SetPath(path.c_str(),
                       url_parse::Component(0, path.length()));
  return url.ReplaceComponents(replacements);
}
