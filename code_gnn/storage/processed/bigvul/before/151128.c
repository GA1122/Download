GURL DevToolsUI::GetProxyURL(const std::string& frontend_url) {
  GURL url(frontend_url);
  if (!url.is_valid() || url.host() != kRemoteFrontendDomain)
    return GURL(kFallbackFrontendURL);
  return GURL(base::StringPrintf("%s://%s/%s/%s",
              content::kChromeDevToolsScheme,
              chrome::kChromeUIDevToolsHost,
              chrome::kChromeUIDevToolsRemotePath,
              url.path().substr(1).c_str()));
}
