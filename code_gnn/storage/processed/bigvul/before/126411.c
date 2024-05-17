bool BrowserWindowGtk::PreHandleKeyboardEvent(
    const NativeWebKeyboardEvent& event, bool* is_keyboard_shortcut) {
  GdkEventKey* os_event = &event.os_event->key;

  if (!os_event || event.type != WebKit::WebInputEvent::RawKeyDown)
    return false;

  if (ExtensionKeybindingRegistryGtk::shortcut_handling_suspended())
    return false;


  int id = GetCustomCommandId(os_event);

  if (id == -1) {
    browser_->command_controller()->SetBlockCommandExecution(true);
    gtk_window_activate_key(window_, os_event);
    id = browser_->command_controller()->GetLastBlockedCommand(NULL);
    browser_->command_controller()->SetBlockCommandExecution(false);
  }

  if (id == -1)
    return false;

  if (browser_->command_controller()->IsReservedCommandOrKey(id, event) &&
      !event.match_edit_command) {
    return chrome::ExecuteCommand(browser_.get(), id);
  }

  DCHECK(is_keyboard_shortcut != NULL);
  *is_keyboard_shortcut = true;

  return false;
}
