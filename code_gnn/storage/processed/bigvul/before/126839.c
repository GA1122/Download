bool BrowserView::PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                         bool* is_keyboard_shortcut) {
  *is_keyboard_shortcut = false;

  if ((event.type != WebKit::WebInputEvent::RawKeyDown) &&
      (event.type != WebKit::WebInputEvent::KeyUp)) {
    return false;
  }

#if defined(OS_WIN) && !defined(USE_AURA)
  if (event.windowsKeyCode == ui::VKEY_F4 &&
      event.type == WebKit::WebInputEvent::RawKeyDown &&
      event.modifiers == NativeWebKeyboardEvent::AltKey) {
    DefWindowProc(event.os_event.hwnd, event.os_event.message,
                  event.os_event.wParam, event.os_event.lParam);
    return true;
  }
#endif

  views::FocusManager* focus_manager = GetFocusManager();
  DCHECK(focus_manager);

  if (focus_manager->shortcut_handling_suspended())
    return false;

  ui::Accelerator accelerator(
      static_cast<ui::KeyboardCode>(event.windowsKeyCode),
      content::GetModifiersFromNativeWebKeyboardEvent(event));
  if (event.type == WebKit::WebInputEvent::KeyUp)
    accelerator.set_type(ui::ET_KEY_RELEASED);


  if (browser_->is_app()) {
    return false;
  }

  chrome::BrowserCommandController* controller = browser_->command_controller();

  controller->SetBlockCommandExecution(true);
  const bool processed = focus_manager->ProcessAccelerator(accelerator);
  const int id = controller->GetLastBlockedCommand(NULL);
  controller->SetBlockCommandExecution(false);

  if (controller->IsReservedCommandOrKey(id, event)) {
    UpdateAcceleratorMetrics(accelerator, id);
    return chrome::ExecuteCommand(browser_.get(), id);
  }

  if (id != -1) {
    if (event.type == WebKit::WebInputEvent::RawKeyDown)
      *is_keyboard_shortcut = true;
  } else if (processed) {
    return true;
  }

  return false;
}
