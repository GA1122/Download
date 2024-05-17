void TabletModeWindowState::DetachState(wm::WindowState* window_state) {
  SetWindowRestoreOverrides(window_state->window(), gfx::Rect(),
                            ui::SHOW_STATE_NORMAL);
}
