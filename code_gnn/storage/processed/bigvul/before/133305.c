void AcceleratorControllerDelegateAura::PerformAction(
    AcceleratorAction action,
    const ui::Accelerator& accelerator) {
  switch (action) {
    case DEBUG_TOGGLE_DESKTOP_BACKGROUND_MODE:
      HandleToggleDesktopBackgroundMode();
      break;
    case DEBUG_TOGGLE_DEVICE_SCALE_FACTOR:
      Shell::GetInstance()->display_manager()->ToggleDisplayScaleFactor();
      break;
    case DEBUG_TOGGLE_ROOT_WINDOW_FULL_SCREEN:
      Shell::GetPrimaryRootWindowController()->ash_host()->ToggleFullScreen();
      break;
    case DEBUG_TOGGLE_SHOW_DEBUG_BORDERS:
      debug::ToggleShowDebugBorders();
      break;
    case DEBUG_TOGGLE_SHOW_FPS_COUNTER:
      debug::ToggleShowFpsCounter();
      break;
    case DEBUG_TOGGLE_SHOW_PAINT_RECTS:
      debug::ToggleShowPaintRects();
      break;
    case FOCUS_SHELF:
      HandleFocusShelf();
      break;
    case LAUNCH_APP_0:
      HandleLaunchAppN(0);
      break;
    case LAUNCH_APP_1:
      HandleLaunchAppN(1);
      break;
    case LAUNCH_APP_2:
      HandleLaunchAppN(2);
      break;
    case LAUNCH_APP_3:
      HandleLaunchAppN(3);
      break;
    case LAUNCH_APP_4:
      HandleLaunchAppN(4);
      break;
    case LAUNCH_APP_5:
      HandleLaunchAppN(5);
      break;
    case LAUNCH_APP_6:
      HandleLaunchAppN(6);
      break;
    case LAUNCH_APP_7:
      HandleLaunchAppN(7);
      break;
    case LAUNCH_LAST_APP:
      HandleLaunchLastApp();
      break;
    case MAGNIFY_SCREEN_ZOOM_IN:
      HandleMagnifyScreen(1);
      break;
    case MAGNIFY_SCREEN_ZOOM_OUT:
      HandleMagnifyScreen(-1);
      break;
    case ROTATE_SCREEN:
      HandleRotateScreen();
      break;
    case ROTATE_WINDOW:
      HandleRotateActiveWindow();
      break;
    case SCALE_UI_DOWN:
      accelerators::ZoomInternalDisplay(false  );
      break;
    case SCALE_UI_RESET:
      accelerators::ResetInternalDisplayZoom();
      break;
    case SCALE_UI_UP:
      accelerators::ZoomInternalDisplay(true  );
      break;
    case SHOW_MESSAGE_CENTER_BUBBLE:
      HandleShowMessageCenterBubble();
      break;
    case SHOW_SYSTEM_TRAY_BUBBLE:
      HandleShowSystemTrayBubble();
      break;
    case TAKE_PARTIAL_SCREENSHOT:
      HandleTakePartialScreenshot(screenshot_delegate_.get());
      break;
    case TAKE_SCREENSHOT:
      HandleTakeScreenshot(screenshot_delegate_.get());
      break;
    case TAKE_WINDOW_SCREENSHOT:
      HandleTakeWindowScreenshot(screenshot_delegate_.get());
      break;
    case UNPIN:
      accelerators::Unpin();
      break;
#if defined(OS_CHROMEOS)
    case DEBUG_ADD_REMOVE_DISPLAY:
      Shell::GetInstance()->display_manager()->AddRemoveDisplay();
      break;
    case DEBUG_TOGGLE_UNIFIED_DESKTOP:
      Shell::GetInstance()->display_manager()->SetUnifiedDesktopEnabled(
          !Shell::GetInstance()->display_manager()->unified_desktop_enabled());
      break;
    case DISABLE_GPU_WATCHDOG:
      Shell::GetInstance()->gpu_support()->DisableGpuWatchdog();
      break;
    case LOCK_PRESSED:
    case LOCK_RELEASED:
      Shell::GetInstance()->power_button_controller()->OnLockButtonEvent(
          action == LOCK_PRESSED, base::TimeTicks());
      break;
    case POWER_PRESSED:   
    case POWER_RELEASED:
      if (!base::SysInfo::IsRunningOnChromeOS()) {
        Shell::GetInstance()->power_button_controller()->OnPowerButtonEvent(
            action == POWER_PRESSED, base::TimeTicks());
      }
      break;
    case SWAP_PRIMARY_DISPLAY:
      HandleSwapPrimaryDisplay();
      break;
    case TOGGLE_MIRROR_MODE:
      HandleToggleMirrorMode();
      break;
    case TOUCH_HUD_CLEAR:
      HandleTouchHudClear();
      break;
    case TOUCH_HUD_MODE_CHANGE:
      HandleTouchHudModeChange();
      break;
    case TOUCH_HUD_PROJECTION_TOGGLE:
      accelerators::ToggleTouchHudProjection();
      break;
#endif
    default:
      break;
  }
}
