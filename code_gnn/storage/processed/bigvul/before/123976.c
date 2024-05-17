void RenderViewImpl::didFirstVisuallyNonEmptyLayout(WebFrame* frame) {
  if (frame != webview()->mainFrame())
    return;

  SkColor bg_color = webwidget_->backgroundColor();

  if (!bg_color)
    bg_color = SK_ColorWHITE;

  if (bg_color != body_background_color_) {
    body_background_color_ = bg_color;
    Send(new ViewHostMsg_DidChangeBodyBackgroundColor(routing_id_, bg_color));
  }
}
