GURL RenderViewImpl::GetURLForGraphicsContext3D() {
  DCHECK(webview());
  WebFrame* main_frame = webview()->MainFrame();
  if (main_frame && main_frame->IsWebLocalFrame())
    return GURL(main_frame->ToWebLocalFrame()->GetDocument().Url());
  else
    return GURL("chrome://gpu/RenderViewImpl::CreateGraphicsContext3D");
}
