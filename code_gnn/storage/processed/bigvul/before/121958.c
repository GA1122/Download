void AppListController::CheckTaskbarOrViewHasFocus() {
  bool was_preserving_focus = preserving_focus_for_taskbar_menu_;
  preserving_focus_for_taskbar_menu_ = false;

  if (!current_view_)
    return;

  HWND jump_list_hwnd = FindWindow(L"DV2ControlHost", NULL);
  HWND taskbar_hwnd = FindWindow(kTrayClassName, NULL);

  HWND app_list_hwnd = GetAppListHWND();

  int swapped = GetSystemMetrics(SM_SWAPBUTTON);
  int left_button = swapped ? VK_RBUTTON : VK_LBUTTON;
  bool left_button_down = GetAsyncKeyState(left_button) < 0;
  int right_button = swapped ? VK_LBUTTON : VK_RBUTTON;
  bool right_button_down = GetAsyncKeyState(right_button) < 0;

  HWND focused_hwnd = GetForegroundWindow();
  if (!focused_hwnd) {
    if (right_button_down || left_button_down)
      return;

    DismissAppList();
    return;
  }

  while (focused_hwnd) {
    if (focused_hwnd == jump_list_hwnd ||
        focused_hwnd == app_list_hwnd) {
      return;
    }

    if (focused_hwnd == taskbar_hwnd) {
      if (right_button_down)
        return;

      if (!was_preserving_focus) {
        preserving_focus_for_taskbar_menu_ = true;
        return;
      }

      break;
    }
    focused_hwnd = GetParent(focused_hwnd);
  }

  if (regain_first_lost_focus_) {
    regain_first_lost_focus_ = false;
    current_view_->GetWidget()->Activate();
    return;
  }

  DismissAppList();
}
