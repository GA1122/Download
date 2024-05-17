bool BrowserWindowGtk::GetCustomFramePrefDefault() {
  ui::WindowManagerName wm_type = ui::GuessWindowManager();
  return (wm_type == ui::WM_BLACKBOX ||
          wm_type == ui::WM_COMPIZ ||
          wm_type == ui::WM_ENLIGHTENMENT ||
          wm_type == ui::WM_METACITY ||
          wm_type == ui::WM_MUTTER ||
          wm_type == ui::WM_OPENBOX ||
          wm_type == ui::WM_XFWM4);
}
