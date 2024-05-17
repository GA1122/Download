void RouteMouseEvent(WebContents* web_contents, blink::WebMouseEvent* event) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  web_contents_impl->GetInputEventRouter()->RouteMouseEvent(
      static_cast<RenderWidgetHostViewBase*>(
          web_contents_impl->GetMainFrame()->GetView()),
      event, ui::LatencyInfo());
}
