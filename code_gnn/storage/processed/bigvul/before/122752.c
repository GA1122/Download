void BrowserPluginGuest::Go(int relative_index) {
  web_contents()->GetController().GoToOffset(relative_index);
}
