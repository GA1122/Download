bool AcceleratorControllerDelegateAura::CanPerformAction(
    AcceleratorAction action,
    const ui::Accelerator& accelerator,
    const ui::Accelerator& previous_accelerator) {
  switch (action) {
    case DEBUG_TOGGLE_DESKTOP_BACKGROUND_MODE:
    case DEBUG_TOGGLE_DEVICE_SCALE_FACTOR:
    case DEBUG_TOGGLE_ROOT_WINDOW_FULL_SCREEN:
    case DEBUG_TOGGLE_SHOW_DEBUG_BORDERS:
    case DEBUG_TOGGLE_SHOW_FPS_COUNTER:
    case DEBUG_TOGGLE_SHOW_PAINT_RECTS:
      return debug::DebugAcceleratorsEnabled();
    case MAGNIFY_SCREEN_ZOOM_IN:
    case MAGNIFY_SCREEN_ZOOM_OUT:
      return CanHandleMagnifyScreen();
    case SCALE_UI_DOWN:
    case SCALE_UI_RESET:
    case SCALE_UI_UP:
      return accelerators::IsInternalDisplayZoomEnabled();
    case SHOW_MESSAGE_CENTER_BUBBLE:
      return CanHandleShowMessageCenterBubble();
    case UNPIN:
      return CanHandleUnpin();

    case FOCUS_SHELF:
    case LAUNCH_APP_0:
    case LAUNCH_APP_1:
    case LAUNCH_APP_2:
    case LAUNCH_APP_3:
    case LAUNCH_APP_4:
    case LAUNCH_APP_5:
    case LAUNCH_APP_6:
    case LAUNCH_APP_7:
    case LAUNCH_LAST_APP:
    case ROTATE_SCREEN:
    case ROTATE_WINDOW:
    case SHOW_SYSTEM_TRAY_BUBBLE:
    case TAKE_PARTIAL_SCREENSHOT:
    case TAKE_SCREENSHOT:
    case TAKE_WINDOW_SCREENSHOT:
      return true;

#if defined(OS_CHROMEOS)
    case DEBUG_ADD_REMOVE_DISPLAY:
    case DEBUG_TOGGLE_UNIFIED_DESKTOP:
      return debug::DebugAcceleratorsEnabled();

    case SWAP_PRIMARY_DISPLAY:
      return display::Screen::GetScreen()->GetNumDisplays() > 1;
    case TOUCH_HUD_CLEAR:
    case TOUCH_HUD_MODE_CHANGE:
      return CanHandleTouchHud();

    case DISABLE_GPU_WATCHDOG:
    case LOCK_PRESSED:
    case LOCK_RELEASED:
    case POWER_PRESSED:
    case POWER_RELEASED:
    case TOGGLE_MIRROR_MODE:
    case TOUCH_HUD_PROJECTION_TOGGLE:
      return true;
#endif

    default:
      NOTREACHED();
      break;
  }
  return false;
}
