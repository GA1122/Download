void BrowserWindowGtk::SaveWindowPosition() {
  ui::WindowShowState show_state = ui::SHOW_STATE_NORMAL;
  if (IsMaximized())
    show_state = ui::SHOW_STATE_MAXIMIZED;
  else if (IsMinimized())
    show_state = ui::SHOW_STATE_MINIMIZED;

  if (chrome::ShouldSaveWindowPlacement(browser_.get()))
    chrome::SaveWindowPlacement(browser_.get(), restored_bounds_, show_state);

  if (!browser_->profile()->GetPrefs())
    return;

  std::string window_name = chrome::GetWindowPlacementKey(browser_.get());
  DictionaryPrefUpdate update(browser_->profile()->GetPrefs(),
                              window_name.c_str());
  DictionaryValue* window_preferences = update.Get();
  window_preferences->SetInteger("left", restored_bounds_.x());
  window_preferences->SetInteger("top", restored_bounds_.y());
  window_preferences->SetInteger("right", restored_bounds_.right());
  window_preferences->SetInteger("bottom", restored_bounds_.bottom());
  window_preferences->SetBoolean("maximized", IsMaximized());

  gfx::Rect work_area(gfx::Screen::GetNativeScreen()->GetDisplayMatching(
      restored_bounds_).work_area());
  window_preferences->SetInteger("work_area_left", work_area.x());
  window_preferences->SetInteger("work_area_top", work_area.y());
  window_preferences->SetInteger("work_area_right", work_area.right());
  window_preferences->SetInteger("work_area_bottom", work_area.bottom());
}
