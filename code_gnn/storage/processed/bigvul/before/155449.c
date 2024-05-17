bool ChromeContentBrowserClient::ShouldForceDownloadResource(
    const GURL& url,
    const std::string& mime_type) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  return extensions::UserScript::IsURLUserScript(url, mime_type);
#else
  return false;
#endif
}
