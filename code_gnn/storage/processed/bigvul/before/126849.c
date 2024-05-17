void BrowserView::SetMetroSnapMode(bool enable) {
  HISTOGRAM_COUNTS("Metro.SnapModeToggle", enable);
  ProcessFullscreen(enable, FOR_METRO, GURL(), FEB_TYPE_NONE);
}
