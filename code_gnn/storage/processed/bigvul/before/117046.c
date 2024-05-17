bool BaseSessionService::ShouldTrackEntry(const GURL& url) {
  return url.is_valid() && url != GURL(chrome::kChromeUIPrintURL);
}
