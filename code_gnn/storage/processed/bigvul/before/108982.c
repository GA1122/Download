void RenderViewImpl::OnSetRendererPrefs(
    const content::RendererPreferences& renderer_prefs) {
  double old_zoom_level = renderer_preferences_.default_zoom_level;
  renderer_preferences_ = renderer_prefs;
  UpdateFontRenderingFromRendererPrefs();
#if defined(TOOLKIT_GTK)
  WebColorName name = WebKit::WebColorWebkitFocusRingColor;
  WebKit::setNamedColors(&name, &renderer_prefs.focus_ring_color, 1);
  WebKit::setCaretBlinkInterval(renderer_prefs.caret_blink_interval);
  ui::NativeTheme::instance()->SetScrollbarColors(
      renderer_prefs.thumb_inactive_color,
      renderer_prefs.thumb_active_color,
      renderer_prefs.track_color);
#endif

#if defined(USE_ASH) || defined(TOOLKIT_GTK)
  if (webview()) {
#if defined(TOOLKIT_GTK)
    webview()->setScrollbarColors(
        renderer_prefs.thumb_inactive_color,
        renderer_prefs.thumb_active_color,
        renderer_prefs.track_color);
#endif
    webview()->setSelectionColors(
        renderer_prefs.active_selection_bg_color,
        renderer_prefs.active_selection_fg_color,
        renderer_prefs.inactive_selection_bg_color,
        renderer_prefs.inactive_selection_fg_color);
    webview()->themeChanged();
  }
#endif

  if (webview() && !webview()->mainFrame()->document().isPluginDocument() &&
      content::ZoomValuesEqual(webview()->zoomLevel(), old_zoom_level)) {
    webview()->setZoomLevel(false, renderer_preferences_.default_zoom_level);
    zoomLevelChanged();
  }
}
