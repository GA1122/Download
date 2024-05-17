std::vector<WebContentsImpl*> WebContentsImpl::GetInnerWebContents() {
  if (browser_plugin_embedder_) {
    std::vector<WebContentsImpl*> inner_contents;
    GetBrowserContext()->GetGuestManager()->ForEachGuest(
        this, base::Bind(&GetInnerWebContentsHelper, &inner_contents));
    return inner_contents;
  }

  return node_.inner_web_contents();
}
