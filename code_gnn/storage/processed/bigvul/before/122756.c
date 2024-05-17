void BrowserPluginGuest::InstallHelper(
    content::RenderViewHost* render_view_host) {
  new BrowserPluginGuestHelper(this, render_view_host);

  notification_registrar_.Add(
      this, content::NOTIFICATION_RESOURCE_RECEIVED_REDIRECT,
      content::Source<content::WebContents>(web_contents()));
}
