bool PanelBrowserView::AcceleratorPressed(
    const ui::Accelerator& accelerator) {
  if (mouse_pressed_ && accelerator.key_code() == ui::VKEY_ESCAPE) {
    OnTitlebarMouseCaptureLost();
    return true;
  }

  if (mouse_dragging_state_ == DRAGGING_STARTED)
    return true;

  return BrowserView::AcceleratorPressed(accelerator);
}
