ui::WindowShowState AdjustShowState(ui::WindowShowState state) {
  switch (state) {
    case ui::SHOW_STATE_NORMAL:
    case ui::SHOW_STATE_MINIMIZED:
    case ui::SHOW_STATE_MAXIMIZED:
    case ui::SHOW_STATE_FULLSCREEN:
      return state;

    case ui::SHOW_STATE_DEFAULT:
    case ui::SHOW_STATE_INACTIVE:
    case ui::SHOW_STATE_END:
      return ui::SHOW_STATE_NORMAL;
  }
  return ui::SHOW_STATE_NORMAL;
}
