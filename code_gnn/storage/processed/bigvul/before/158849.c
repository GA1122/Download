void Browser::RendererResponsive(
    WebContents* source,
    content::RenderWidgetHost* render_widget_host) {
  TabDialogs::FromWebContents(source)->HideHungRendererDialog(
      render_widget_host);
}
