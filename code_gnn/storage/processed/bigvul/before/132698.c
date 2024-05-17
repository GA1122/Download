WebThemeEngine* LayoutTestContentRendererClient::OverrideThemeEngine() {
  return LayoutTestRenderProcessObserver::GetInstance()
      ->test_interfaces()
      ->ThemeEngine();
}
