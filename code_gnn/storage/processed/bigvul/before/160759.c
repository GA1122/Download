void RenderViewImpl::ApplyWebPreferencesInternal(
    const WebPreferences& prefs,
    blink::WebView* web_view,
    CompositorDependencies* compositor_deps) {
  ApplyWebPreferences(prefs, web_view);
}
