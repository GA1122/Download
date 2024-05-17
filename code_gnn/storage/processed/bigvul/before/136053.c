std::string ExtensionWebContentsObserver::GetExtensionIdFromFrame(
    content::RenderFrameHost* render_frame_host) const {
  content::SiteInstance* site_instance = render_frame_host->GetSiteInstance();
  GURL url = render_frame_host->GetLastCommittedURL();
  if (!url.is_empty()) {
    if (site_instance->GetSiteURL().GetOrigin() != url.GetOrigin())
      return std::string();
  } else {
    url = site_instance->GetSiteURL();
  }

  return url.SchemeIs(kExtensionScheme) ? url.host() : std::string();
}
