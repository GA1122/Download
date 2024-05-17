void SetLockedFullscreenState(Browser* browser, bool locked) {
  UMA_HISTOGRAM_BOOLEAN("Extensions.LockedFullscreenStateRequest", locked);

  aura::Window* window = browser->window()->GetNativeWindow();
  window->SetProperty(ash::kWindowPinTypeKey,
                      locked ? ash::mojom::WindowPinType::TRUSTED_PINNED
                             : ash::mojom::WindowPinType::NONE);

  browser->command_controller()->LockedFullscreenStateChanged();

  if (!features::IsMultiProcessMash())
    ChromeScreenshotGrabber::Get()->set_screenshots_allowed(!locked);

  ui::Clipboard::GetForCurrentThread()->Clear(ui::CLIPBOARD_TYPE_COPY_PASTE);
  content::DevToolsAgentHost::DetachAllClients();
}
