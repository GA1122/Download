bool PanelBrowserView::OnTitlebarMouseReleased(panel::ClickModifier modifier) {
  if (mouse_dragging_state_ != NO_DRAGGING) {
    if (panel_->IsMinimized() && panel_->IsActive())
      panel_->Deactivate();

    if (mouse_dragging_state_ == DRAGGING_STARTED) {
      if (old_focused_view_) {
        GetFocusManager()->SetFocusedView(old_focused_view_);
        old_focused_view_ = NULL;
      }
      return EndDragging(false);
    }

    if (mouse_dragging_state_ != NO_DRAGGING)
      return true;
  }

  panel_->OnTitlebarClicked(modifier);
  return true;
}
