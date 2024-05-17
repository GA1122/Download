void WebContentsImpl::SetBrowserPluginGuest(BrowserPluginGuest* guest) {
  CHECK(!browser_plugin_guest_);
  CHECK(guest);
  browser_plugin_guest_.reset(guest);
}
