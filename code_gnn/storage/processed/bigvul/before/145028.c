void CustomButton::ShowContextMenu(const gfx::Point& p,
                                   ui::MenuSourceType source_type) {
  if (!context_menu_controller())
    return;

  if (state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
  View::ShowContextMenu(p, source_type);
}
