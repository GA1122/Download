void WebContentsImpl::BrowserPluginGuestWillDetach() {
  WebContentsImpl* outermost = GetOutermostWebContents();
  if (this != outermost && ContainsOrIsFocusedWebContents())
    outermost->SetAsFocusedWebContentsIfNecessary();
}
