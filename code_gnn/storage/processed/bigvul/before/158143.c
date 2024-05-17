  const net::test_server::HttpRequest* FindAccumulatedRequest(
      const GURL& url_to_find) {
    DCHECK(url_to_find.SchemeIsHTTPOrHTTPS());

    auto it = std::find_if(
        accumulated_requests_.begin(), accumulated_requests_.end(),
        [&url_to_find](const net::test_server::HttpRequest& request) {
          return request.GetURL() == url_to_find;
        });
    if (it == accumulated_requests_.end())
      return nullptr;
    return &*it;
  }
