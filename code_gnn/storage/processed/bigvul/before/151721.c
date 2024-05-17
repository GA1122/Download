void Browser::RendererResponsive(WebContents* source) {
  TabDialogs::FromWebContents(source)->HideHungRendererDialog();
}
