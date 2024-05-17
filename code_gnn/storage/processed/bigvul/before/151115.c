void DevToolsWindow::ShowCertificateViewer(
    scoped_refptr<net::X509Certificate> certificate) {
  WebContents* inspected_contents = is_docked_ ?
      GetInspectedWebContents() : main_web_contents_;
  Browser* browser = NULL;
  int tab = 0;
  if (!FindInspectedBrowserAndTabIndex(inspected_contents, &browser, &tab))
    return;
  gfx::NativeWindow parent = browser->window()->GetNativeWindow();
  ::ShowCertificateViewer(inspected_contents, parent, certificate.get());
}
