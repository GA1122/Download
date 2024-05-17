void TabletModeWindowState::OnWMEvent(wm::WindowState* window_state,
                                      const wm::WMEvent* event) {
  if (ignore_wm_events_) {
    return;
  }

  switch (event->type()) {
    case wm::WM_EVENT_TOGGLE_FULLSCREEN:
      ToggleFullScreen(window_state, window_state->delegate());
      break;
    case wm::WM_EVENT_FULLSCREEN:
      UpdateWindow(window_state, WindowStateType::kFullscreen,
                   true  );
      break;
    case wm::WM_EVENT_PIN:
      if (!Shell::Get()->screen_pinning_controller()->IsPinned())
        UpdateWindow(window_state, WindowStateType::kPinned,
                     true  );
      break;
    case wm::WM_EVENT_PIP:
      if (!window_state->IsPip()) {
        UpdateWindow(window_state, WindowStateType::kPip, true  );
      }
      break;
    case wm::WM_EVENT_TRUSTED_PIN:
      if (!Shell::Get()->screen_pinning_controller()->IsPinned())
        UpdateWindow(window_state, WindowStateType::kTrustedPinned,
                     true  );
      break;
    case wm::WM_EVENT_TOGGLE_MAXIMIZE_CAPTION:
    case wm::WM_EVENT_TOGGLE_VERTICAL_MAXIMIZE:
    case wm::WM_EVENT_TOGGLE_HORIZONTAL_MAXIMIZE:
    case wm::WM_EVENT_TOGGLE_MAXIMIZE:
    case wm::WM_EVENT_CYCLE_SNAP_LEFT:
    case wm::WM_EVENT_CYCLE_SNAP_RIGHT:
    case wm::WM_EVENT_CENTER:
    case wm::WM_EVENT_NORMAL:
    case wm::WM_EVENT_MAXIMIZE:
      UpdateWindow(window_state, GetMaximizedOrCenteredWindowType(window_state),
                   true  );
      return;
    case wm::WM_EVENT_SNAP_LEFT:
      window_state->set_bounds_changed_by_user(true);
      UpdateWindow(window_state,
                   GetSnappedWindowStateType(window_state,
                                             WindowStateType::kLeftSnapped),
                   false  );
      return;
    case wm::WM_EVENT_SNAP_RIGHT:
      window_state->set_bounds_changed_by_user(true);
      UpdateWindow(window_state,
                   GetSnappedWindowStateType(window_state,
                                             WindowStateType::kRightSnapped),
                   false  );
      return;
    case wm::WM_EVENT_MINIMIZE:
      UpdateWindow(window_state, WindowStateType::kMinimized,
                   true  );
      return;
    case wm::WM_EVENT_SHOW_INACTIVE:
    case wm::WM_EVENT_SYSTEM_UI_AREA_CHANGED:
      return;
    case wm::WM_EVENT_SET_BOUNDS: {
      gfx::Rect bounds_in_parent =
          (static_cast<const wm::SetBoundsEvent*>(event))->requested_bounds();
      if (bounds_in_parent.IsEmpty())
        return;

      if (wm::IsDraggingTabs(window_state->window()) ||
          IsTabDraggingSourceWindow(window_state->window())) {
        window_state->SetBoundsDirect(bounds_in_parent);
      } else if (current_state_type_ == WindowStateType::kMaximized) {
        window_state->SetRestoreBoundsInParent(bounds_in_parent);
      } else if (current_state_type_ != WindowStateType::kMinimized &&
                 current_state_type_ != WindowStateType::kFullscreen &&
                 current_state_type_ != WindowStateType::kPinned &&
                 current_state_type_ != WindowStateType::kTrustedPinned &&
                 current_state_type_ != WindowStateType::kLeftSnapped &&
                 current_state_type_ != WindowStateType::kRightSnapped) {
        bounds_in_parent = GetCenteredBounds(bounds_in_parent, window_state);
        if (bounds_in_parent != window_state->window()->bounds()) {
          const wm::SetBoundsEvent* bounds_event =
              static_cast<const wm::SetBoundsEvent*>(event);
          if (window_state->window()->IsVisible() && bounds_event->animate())
            window_state->SetBoundsDirectAnimated(bounds_in_parent);
          else
            window_state->SetBoundsDirect(bounds_in_parent);
        }
      }
      break;
    }
    case wm::WM_EVENT_ADDED_TO_WORKSPACE:
      if (current_state_type_ != WindowStateType::kMaximized &&
          current_state_type_ != WindowStateType::kFullscreen &&
          current_state_type_ != WindowStateType::kMinimized) {
        WindowStateType new_state =
            GetMaximizedOrCenteredWindowType(window_state);
        UpdateWindow(window_state, new_state, true  );
      }
      break;
    case wm::WM_EVENT_WORKAREA_BOUNDS_CHANGED:
      if (current_state_type_ != WindowStateType::kMinimized)
        UpdateBounds(window_state, true  );
      break;
    case wm::WM_EVENT_DISPLAY_BOUNDS_CHANGED:
      if (current_state_type_ != WindowStateType::kMinimized)
        UpdateBounds(window_state, false  );
      break;
  }
}
