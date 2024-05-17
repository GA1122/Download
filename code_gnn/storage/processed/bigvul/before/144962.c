void RenderWidgetHostViewAura::OnWindowFocused(aura::Window* gained_focus,
                                               aura::Window* lost_focus) {
  DCHECK(window_ == gained_focus || window_ == lost_focus);
  if (window_ == gained_focus) {
    if (host_->ignore_input_events())
      return;

    host_->GotFocus();
    host_->SetActive(true);

    ui::InputMethod* input_method = GetInputMethod();
    if (input_method) {
      input_method->SetFocusedTextInputClient(this);
    }

    BrowserAccessibilityManager* manager =
        host_->GetRootBrowserAccessibilityManager();
    if (manager)
      manager->OnWindowFocused();
  } else if (window_ == lost_focus) {
    host_->SetActive(false);
    host_->Blur();

    DetachFromInputMethod();

    selection_controller_->HideAndDisallowShowingAutomatically();

    if (overscroll_controller_)
      overscroll_controller_->Cancel();

    BrowserAccessibilityManager* manager =
        host_->GetRootBrowserAccessibilityManager();
    if (manager)
      manager->OnWindowBlurred();

    display::Screen* screen = display::Screen::GetScreen();
    bool focusing_other_display =
        gained_focus && screen->GetNumDisplays() > 1 &&
        (screen->GetDisplayNearestWindow(window_).id() !=
         screen->GetDisplayNearestWindow(gained_focus).id());
    if (is_fullscreen_ && !in_shutdown_ && !focusing_other_display) {
#if defined(OS_WIN)
      if (!gained_focus) {
        POINT point = {0};
        ::GetCursorPos(&point);
        if (screen->GetDisplayNearestWindow(window_).id() !=
            screen->GetDisplayNearestPoint(gfx::Point(point)).id())
          return;
      }
#endif
      Shutdown();
      return;
    }

    if (popup_child_host_view_)
      popup_child_host_view_->Shutdown();
  }
}
