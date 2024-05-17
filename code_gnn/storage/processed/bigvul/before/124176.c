bool ContentBrowserClient::AllowPepperSocketAPI(
    BrowserContext* browser_context,
    const GURL& url,
    const SocketPermissionRequest& params) {
  return false;
}
