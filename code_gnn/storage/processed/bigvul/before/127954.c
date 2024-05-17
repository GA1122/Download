void BrowserViewRenderer::RegisterWithWebContents(
    content::WebContents* web_contents) {
  web_contents->SetUserData(kBrowserViewRendererUserDataKey,
                            new BrowserViewRendererUserData(this));
}
