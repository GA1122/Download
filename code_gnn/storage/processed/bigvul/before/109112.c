void RenderViewImpl::didReceiveTitle(WebFrame* frame, const WebString& title,
                                     WebTextDirection direction) {
  UpdateTitle(frame, title, direction);

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
