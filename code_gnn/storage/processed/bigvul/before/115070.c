void TestingAutomationProvider::WindowSimulateKeyPress(
    const IPC::Message& message,
    int handle,
    int key,
    int flags) {
  if (!window_tracker_->ContainsHandle(handle))
    return;

  gfx::NativeWindow window = window_tracker_->GetResource(handle);
  ui_controls::SendKeyPress(window, static_cast<ui::KeyboardCode>(key),
                            ((flags & ui::EF_CONTROL_DOWN) ==
                             ui::EF_CONTROL_DOWN),
                            ((flags & ui::EF_SHIFT_DOWN) ==
                             ui::EF_SHIFT_DOWN),
                            ((flags & ui::EF_ALT_DOWN) ==
                             ui::EF_ALT_DOWN),
                            ((flags & ui::EF_COMMAND_DOWN) ==
                             ui::EF_COMMAND_DOWN));
}
