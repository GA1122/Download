void ChromeContentBrowserClient::AddCertificate(
    net::URLRequest* request,
    net::CertificateMimeType cert_type,
    const void* cert_data,
    size_t cert_size,
    int render_process_id,
    int render_view_id) {
  chrome::SSLAddCertificate(request, cert_type, cert_data, cert_size,
      render_process_id, render_view_id);
}
