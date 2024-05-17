bool RenderThreadImpl::IsWebFrameValid(WebKit::WebFrame* web_frame) {
  if (!web_frame)
    return false;  

  RenderViewImpl* render_view = RenderViewImpl::FromWebView(web_frame->view());
  if (!render_view)
    return false;  

  return true;
}
