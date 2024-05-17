DesktopWindowTreeHostX11::CreateDragDropClient(
    DesktopNativeCursorManager* cursor_manager) {
  drag_drop_client_ = new DesktopDragDropClientAuraX11(
      window(), cursor_manager, xdisplay_, xwindow_);
  drag_drop_client_->Init();
  return base::WrapUnique(drag_drop_client_);
}
