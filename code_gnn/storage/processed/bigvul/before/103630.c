void ChromeContentBrowserClient::ShowClientCertificateRequestDialog(
    int render_process_id,
    int render_view_id,
    SSLClientAuthHandler* handler) {
  TabContents* tab = tab_util::GetTabContentsByID(
      render_process_id, render_view_id);
  if (!tab) {
    NOTREACHED();
    return;
  }

  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(tab);
  wrapper->ssl_helper()->ShowClientCertificateRequestDialog(handler);
}
