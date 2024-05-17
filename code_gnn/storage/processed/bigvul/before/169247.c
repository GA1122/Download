RenderWidgetHost* GetFocusedRenderWidgetHost(WebContents* web_contents) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  return web_contents_impl->GetFocusedRenderWidgetHost(
      web_contents_impl->GetMainFrame()->GetRenderWidgetHost());
}
