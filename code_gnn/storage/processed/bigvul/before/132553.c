void ShellContentBrowserClient::OverrideWebkitPrefs(
    RenderViewHost* render_view_host,
    WebPreferences* prefs) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kRunLayoutTest))
    return;
  WebKitTestController::Get()->OverrideWebkitPrefs(prefs);
}
