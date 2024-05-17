void Browser::RendererResponsive(WebContents* source) {
  chrome::HideHungRendererDialog(source);
}
