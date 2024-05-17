void ChromeContentBrowserClient::RenderViewHostCreated(
    RenderViewHost* render_view_host) {

  SiteInstance* site_instance = render_view_host->GetSiteInstance();
  Profile* profile = Profile::FromBrowserContext(
      site_instance->GetBrowserContext());

  new ChromeRenderViewHostObserver(render_view_host,
                                   profile->GetNetworkPredictor());
  new extensions::MessageHandler(render_view_host);
}
