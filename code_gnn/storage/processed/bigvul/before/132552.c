void ShellContentBrowserClient::OpenURL(
    BrowserContext* browser_context,
    const OpenURLParams& params,
    const base::Callback<void(WebContents*)>& callback) {
  callback.Run(Shell::CreateNewWindow(browser_context,
                                      params.url,
                                      nullptr,
                                      gfx::Size())->web_contents());
}
