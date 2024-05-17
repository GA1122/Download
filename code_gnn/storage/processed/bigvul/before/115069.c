void TestingAutomationProvider::WindowSimulateClick(const IPC::Message& message,
                                                    int handle,
                                                    const gfx::Point& click,
                                                    int flags) {
  if (window_tracker_->ContainsHandle(handle)) {
    ui_controls::SendMouseMoveNotifyWhenDone(
        click.x(), click.y(), base::Bind(&SendMouseClick, flags));
  }
}
