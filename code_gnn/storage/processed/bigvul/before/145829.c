HeadlessDevToolsManagerDelegate::CreateNewTarget(const GURL& url) {
  if (!browser_)
    return nullptr;

  HeadlessBrowserContext* context = browser_->GetDefaultBrowserContext();
  HeadlessWebContentsImpl* web_contents_impl = HeadlessWebContentsImpl::From(
      context->CreateWebContentsBuilder()
          .SetInitialURL(url)
          .SetWindowSize(browser_->options()->window_size)
          .Build());
  return content::DevToolsAgentHost::GetOrCreateFor(
      web_contents_impl->web_contents());
}
