void Browser::ShowCertificateViewerInDevTools(
    content::WebContents* web_contents,
    scoped_refptr<net::X509Certificate> certificate) {
  DevToolsWindow* devtools_window =
      DevToolsWindow::GetInstanceForInspectedWebContents(web_contents);
  if (devtools_window)
    devtools_window->ShowCertificateViewer(certificate);
}
