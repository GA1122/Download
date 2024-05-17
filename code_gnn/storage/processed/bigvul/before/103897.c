void RenderView::didReceiveTitle(WebFrame* frame, const WebString& title,
                                 WebTextDirection direction) {
  UpdateTitle(frame, title);

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
