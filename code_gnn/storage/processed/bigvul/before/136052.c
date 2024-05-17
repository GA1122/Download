std::string ExtensionWebContentsObserver::GetExtensionId(
    content::RenderViewHost* render_view_host) {
  const GURL& site = render_view_host->GetSiteInstance()->GetSiteURL();

  if (!site.SchemeIs(kExtensionScheme))
    return std::string();

  return site.host();
}
