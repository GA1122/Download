bool ExtensionDevToolsClientHost::MayAttachToRenderer(
    content::RenderFrameHost* render_frame_host,
    bool is_webui) {
  if (is_webui)
    return false;

  if (!render_frame_host)
    return true;

  std::string error;
  const GURL& site_instance_url =
      render_frame_host->GetSiteInstance()->GetSiteURL();

  if (site_instance_url.is_empty() || site_instance_url == "about:") {
    return true;
  }

  return ExtensionCanAttachToURL(*extension_, site_instance_url, profile_,
                                 &error);
}
