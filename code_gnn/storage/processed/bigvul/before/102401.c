std::string GetHostAndPort(const GURL& url) {
  return base::StringPrintf("%s:%d", url.host().c_str(),
                            url.EffectiveIntPort());
}
