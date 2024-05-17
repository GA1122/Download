void DevToolsWindow::ShowCertificateViewerInDevTools(
    content::WebContents* web_contents,
    scoped_refptr<net::X509Certificate> certificate) {
  ShowCertificateViewer(certificate);
}
