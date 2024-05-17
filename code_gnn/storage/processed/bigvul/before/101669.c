ui::WindowShowState Browser::GetSavedWindowShowState() const {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kStartMaximized))
    return ui::SHOW_STATE_MAXIMIZED;

  if (show_state_ != ui::SHOW_STATE_DEFAULT)
    return show_state_;

  gfx::Rect restored_bounds;
  bool maximized = false;
  WindowSizer::GetBrowserWindowBounds(app_name_, restored_bounds, this,
                                      &restored_bounds, &maximized);
  return maximized ? ui::SHOW_STATE_MAXIMIZED : ui::SHOW_STATE_NORMAL;
}
