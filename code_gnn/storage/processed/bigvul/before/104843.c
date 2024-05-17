  bool UrlIsPendingInPrerenderManager(const std::string& html_file) {
    GURL dest_url = test_server()->GetURL(html_file);
    return (prerender_manager()->FindPendingEntry(dest_url) != NULL);
  }
