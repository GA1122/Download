void ChromeRenderProcessObserver::OnToggleWebKitSharedTimer(bool suspend) {
  RenderThread::Get()->ToggleWebKitSharedTimer(suspend);
}
