void BrowserView::OnWidgetActivationChanged(views::Widget* widget,
                                            bool active) {
  if (browser_->window()) {
    if (active)
      BrowserList::SetLastActive(browser_.get());
    else
      BrowserList::NotifyBrowserNoLongerActive(browser_.get());
  }

  if (!extension_keybinding_registry_ &&
      GetFocusManager()) {   
    extension_keybinding_registry_.reset(new ExtensionKeybindingRegistryViews(
        browser_->profile(), GetFocusManager(),
        extensions::ExtensionKeybindingRegistry::ALL_EXTENSIONS, this));
  }

  extensions::ExtensionCommandsGlobalRegistry* registry =
      extensions::ExtensionCommandsGlobalRegistry::Get(browser_->profile());
  if (active) {
    registry->set_registry_for_active_window(
        extension_keybinding_registry_.get());
  } else if (registry->registry_for_active_window() ==
             extension_keybinding_registry_.get()) {
    registry->set_registry_for_active_window(nullptr);
  }

  immersive_mode_controller()->OnWidgetActivationChanged(widget, active);
}
