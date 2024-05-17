std::vector<RenderWidgetHostView*> GetInputEventRouterRenderWidgetHostViews(
    WebContents* web_contents) {
  return static_cast<WebContentsImpl*>(web_contents)
      ->GetInputEventRouter()
      ->GetRenderWidgetHostViewsForTests();
}
