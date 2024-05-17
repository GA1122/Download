 bool PluginInstance::ConvertPoint(double source_x, double source_y,
                                  NPCoordinateSpace source_space,
                                  double* dest_x, double* dest_y,
                                  NPCoordinateSpace dest_space) {
#if defined(OS_MACOSX)
  CGRect main_display_bounds = CGDisplayBounds(CGMainDisplayID());

  double flipped_screen_x = source_x;
  double flipped_screen_y = source_y;
  switch(source_space) {
    case NPCoordinateSpacePlugin:
      flipped_screen_x += plugin_origin_.x();
      flipped_screen_y += plugin_origin_.y();
      break;
    case NPCoordinateSpaceScreen:
      flipped_screen_y = main_display_bounds.size.height - flipped_screen_y;
    case NPCoordinateSpaceFlippedScreen:
      break;
    case NPCoordinateSpaceWindow:
    case NPCoordinateSpaceFlippedWindow:
      NOTIMPLEMENTED();
      return false;
    default:
      NOTREACHED();
      return false;
  }

  double target_x = flipped_screen_x;
  double target_y = flipped_screen_y;
  switch(dest_space) {
    case NPCoordinateSpacePlugin:
      target_x -= plugin_origin_.x();
      target_y -= plugin_origin_.y();
      break;
    case NPCoordinateSpaceScreen:
      target_y = main_display_bounds.size.height - flipped_screen_y;
    case NPCoordinateSpaceFlippedScreen:
      break;
    case NPCoordinateSpaceWindow:
    case NPCoordinateSpaceFlippedWindow:
      NOTIMPLEMENTED();
      return false;
    default:
      NOTREACHED();
      return false;
  }

  if (dest_x)
    *dest_x = target_x;
  if (dest_y)
    *dest_y = target_y;
  return true;
#else
  NOTIMPLEMENTED();
  return false;
 #endif
 }
