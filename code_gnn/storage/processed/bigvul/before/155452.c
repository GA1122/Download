bool ChromeContentBrowserClient::ShouldStayInParentProcessForNTP(
    const GURL& url,
    SiteInstance* parent_site_instance) {
  return url.SchemeIs(chrome::kChromeSearchScheme) &&
         IsNTPSiteInstance(parent_site_instance);
}
