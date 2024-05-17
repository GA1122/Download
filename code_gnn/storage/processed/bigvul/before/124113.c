bool HandleWebUIReverse(GURL* url, content::BrowserContext* browser_context) {
  if (!url->is_valid() || !url->SchemeIs(chrome::kChromeUIScheme))
    return false;

  return RemoveUberHost(url);
}
