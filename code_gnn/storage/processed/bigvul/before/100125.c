bool BrowserActionButton::OnMousePressed(const views::MouseEvent& e) {
  showing_context_menu_ = e.IsRightMouseButton();
  if (showing_context_menu_) {
    SetButtonPushed();

    gfx::Point point = gfx::Point(0, 0);
    ConvertPointToScreen(this, &point);

    point.Offset(0, height());

    if (!context_menu_.get())
      context_menu_.reset(new ExtensionActionContextMenu());
    context_menu_->Run(extension(), point);

    SetButtonNotPushed();
    return false;
  } else if (IsPopup()) {
    return MenuButton::OnMousePressed(e);
  }
  return TextButton::OnMousePressed(e);
}
