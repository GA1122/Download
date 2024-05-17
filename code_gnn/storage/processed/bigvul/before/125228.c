void RaiseInfobarForBlocked3DContentOnUIThread(
    int render_process_id,
    int render_view_id,
    const GURL& url,
    content::ThreeDAPIType requester) {
  RenderViewHost* rvh = RenderViewHost::FromID(
      render_process_id, render_view_id);
  if (!rvh)
    return;
  WebContentsImpl* web_contents = static_cast<WebContentsImpl*>(
      WebContents::FromRenderViewHost(rvh));
  if (!web_contents)
    return;
  web_contents->DidBlock3DAPIs(url, requester);
}
