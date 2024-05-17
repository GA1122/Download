GURL GetFeedUrl(net::TestServer* server, const std::string& feed_page,
                bool direct_url, std::string extension_id) {
  GURL feed_url = server->GetURL(feed_page);
  if (direct_url) {
    return GURL(std::string(chrome::kExtensionScheme) +
        chrome::kStandardSchemeSeparator +
        extension_id + std::string(kSubscribePage) + std::string("?") +
        feed_url.spec() + std::string("&synchronous"));
  } else {
    return GURL(feed_url.spec());
  }
}
