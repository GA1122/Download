void SimulateUnresponsiveRenderer(WebContents* web_contents,
                                  RenderWidgetHost* widget) {
  static_cast<WebContentsImpl*>(web_contents)
      ->RendererUnresponsive(RenderWidgetHostImpl::From(widget));
}
