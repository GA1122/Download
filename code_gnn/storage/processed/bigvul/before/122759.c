void BrowserPluginGuest::Reload() {
  web_contents()->GetController().Reload(false);
}
