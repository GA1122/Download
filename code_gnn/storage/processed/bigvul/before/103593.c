void ChromeContentBrowserClient::AddNewCertificate(
    net::URLRequest* request,
    net::X509Certificate* cert,
    int render_process_id,
    int render_view_id) {
  new SSLAddCertHandler(request, cert, render_process_id, render_view_id);
}
