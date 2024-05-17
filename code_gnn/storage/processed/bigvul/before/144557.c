bool WebContentsImpl::GotResponseToLockMouseRequest(bool allowed) {
  if (GetBrowserPluginGuest())
    return GetBrowserPluginGuest()->LockMouse(allowed);

  return GetRenderViewHost()
             ? GetRenderViewHost()->GetWidget()->GotResponseToLockMouseRequest(
                   allowed)
             : false;
}
