void PrepareFrameAndViewForPrint::CopySelection(
    const WebPreferences& preferences) {
  ResizeForPrinting();
  std::string url_str = "data:text/html;charset=utf-8,";
  url_str.append(
      net::EscapeQueryParamValue(frame()->SelectionAsMarkup().Utf8(), false));
  RestoreSize();
  WebPreferences prefs = preferences;
  prefs.javascript_enabled = false;

  blink::WebView* web_view = blink::WebView::Create(
        this, blink::mojom::PageVisibilityState::kVisible,
        nullptr);
  owns_web_view_ = true;
  content::RenderView::ApplyWebPreferences(prefs, web_view);
  blink::WebLocalFrame* main_frame =
      blink::WebLocalFrame::CreateMainFrame(web_view, this, nullptr, nullptr);
  frame_.Reset(main_frame);
  blink::WebFrameWidget::Create(this, main_frame);
  node_to_print_.Reset();

  blink::WebURLRequest request = blink::WebURLRequest(GURL(url_str));
  frame()->LoadRequest(request);
}
