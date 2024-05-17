void BrowserEventRouter::OnBrowserSetLastActive(Browser* browser) {
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  if (service) {
    service->window_event_router()->OnActiveWindowChanged(
        browser ? browser->extension_window_controller() : NULL);
  }
}
