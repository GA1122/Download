void FastSetActiveURL(const GURL& url, size_t url_hash) {
  if (url.is_empty())
    return;
  static size_t g_last_url_hash = 0;
  if (url_hash != g_last_url_hash) {
    g_last_url_hash = url_hash;
    GetContentClient()->SetActiveURL(url);
  }
}
