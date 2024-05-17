Browser* Browser::CreateNewStripWithContents(
    TabContentsWrapper* detached_contents,
    const gfx::Rect& window_bounds,
    const DockInfo& dock_info,
    bool maximize) {
  DCHECK(CanSupportWindowFeature(FEATURE_TABSTRIP));

  gfx::Rect new_window_bounds = window_bounds;
  if (dock_info.GetNewWindowBounds(&new_window_bounds, &maximize))
    dock_info.AdjustOtherWindowBounds();

  Browser* browser = new Browser(TYPE_TABBED, profile_);
  browser->set_override_bounds(new_window_bounds);
  browser->set_show_state(
      maximize ? ui::SHOW_STATE_MAXIMIZED : ui::SHOW_STATE_NORMAL);
  browser->InitBrowserWindow();
  browser->tabstrip_model()->AppendTabContents(detached_contents, true);
  browser->LoadingStateChanged(detached_contents->tab_contents());
  return browser;
}
