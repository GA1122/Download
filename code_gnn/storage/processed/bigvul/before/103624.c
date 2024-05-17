bool ChromeContentBrowserClient::IsURLSameAsAnySiteInstance(const GURL& url) {
  return url == GURL(chrome::kChromeUICrashURL) ||
         url == GURL(chrome::kChromeUIKillURL) ||
         url == GURL(chrome::kChromeUIHangURL) ||
         url == GURL(chrome::kChromeUIShorthangURL);
}
