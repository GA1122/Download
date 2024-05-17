bool PrintWebViewHelper::GetPrintFrame(WebKit::WebFrame** frame) {
  DCHECK(frame);
  DCHECK(render_view()->webview());
  if (!render_view()->webview())
    return false;

  *frame = render_view()->webview()->focusedFrame()->hasSelection() ?
      render_view()->webview()->focusedFrame() :
      render_view()->webview()->mainFrame();
  return true;
}
