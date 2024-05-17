  GURL GetCrossSiteURL(const std::string& path) {
    GURL cross_site_url(embedded_test_server()->GetURL(path));
    return cross_site_url.ReplaceComponents(replace_host_);
  }
