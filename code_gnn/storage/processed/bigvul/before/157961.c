void RenderViewImpl::OnSetRendererPrefs(
    const RendererPreferences& renderer_prefs) {
  std::string old_accept_languages = renderer_preferences_.accept_languages;

  renderer_preferences_ = renderer_prefs;

  renderer_preference_watchers_.ForAllPtrs(
      [&renderer_prefs](mojom::RendererPreferenceWatcher* watcher) {
        watcher->NotifyUpdate(renderer_prefs);
      });

  UpdateFontRenderingFromRendererPrefs();
  UpdateThemePrefs();
  blink::SetCaretBlinkInterval(renderer_prefs.caret_blink_interval);

#if BUILDFLAG(USE_DEFAULT_RENDER_THEME)
  if (renderer_prefs.use_custom_colors) {
    blink::SetFocusRingColor(renderer_prefs.focus_ring_color);

    if (webview()) {
      webview()->SetSelectionColors(renderer_prefs.active_selection_bg_color,
                                    renderer_prefs.active_selection_fg_color,
                                    renderer_prefs.inactive_selection_bg_color,
                                    renderer_prefs.inactive_selection_fg_color);
      webview()->ThemeChanged();
    }
  }
#endif   

  if (webview() &&
      old_accept_languages != renderer_preferences_.accept_languages) {
    webview()->AcceptLanguagesChanged();
  }
}
