bool ChromeContentBrowserClient::ShouldUseProcessPerSite(
    content::BrowserContext* browser_context, const GURL& effective_url) {

  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile)
    return false;

#if !defined(OS_ANDROID)
  if (search::ShouldUseProcessPerSiteForInstantURL(effective_url, profile))
    return true;
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
  return ChromeContentBrowserClientExtensionsPart::ShouldUseProcessPerSite(
      profile, effective_url);
#else
  return false;
#endif
}
