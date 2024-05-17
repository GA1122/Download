void WebUILoginView::OpenProxySettings() {
  ProxySettingsDialog* dialog =
      new ProxySettingsDialog(NULL, GetNativeWindow());
  dialog->Show();
}
