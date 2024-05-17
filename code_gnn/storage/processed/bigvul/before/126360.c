bool BrowserWindowGtk::GetWindowEdge(int x, int y, GdkWindowEdge* edge) {
  if (!UseCustomFrame())
    return false;

  if (IsMaximized() || IsFullscreen())
    return false;

  if (x < kFrameBorderThickness) {
    if (y < kResizeAreaCornerSize - kTopResizeAdjust) {
      *edge = GDK_WINDOW_EDGE_NORTH_WEST;
    } else if (y < bounds_.height() - kResizeAreaCornerSize) {
      *edge = GDK_WINDOW_EDGE_WEST;
    } else {
      *edge = GDK_WINDOW_EDGE_SOUTH_WEST;
    }
    return true;
  } else if (x < bounds_.width() - kFrameBorderThickness) {
    if (y < kFrameBorderThickness - kTopResizeAdjust) {
      if (x < kResizeAreaCornerSize) {
        *edge = GDK_WINDOW_EDGE_NORTH_WEST;
      } else if (x < bounds_.width() - kResizeAreaCornerSize) {
        *edge = GDK_WINDOW_EDGE_NORTH;
      } else {
        *edge = GDK_WINDOW_EDGE_NORTH_EAST;
      }
    } else if (y < bounds_.height() - kFrameBorderThickness) {
      return false;
    } else {
      if (x < kResizeAreaCornerSize) {
        *edge = GDK_WINDOW_EDGE_SOUTH_WEST;
      } else if (x < bounds_.width() - kResizeAreaCornerSize) {
        *edge = GDK_WINDOW_EDGE_SOUTH;
      } else {
        *edge = GDK_WINDOW_EDGE_SOUTH_EAST;
      }
    }
    return true;
  } else {
    if (y < kResizeAreaCornerSize - kTopResizeAdjust) {
      *edge = GDK_WINDOW_EDGE_NORTH_EAST;
    } else if (y < bounds_.height() - kResizeAreaCornerSize) {
      *edge = GDK_WINDOW_EDGE_EAST;
    } else {
      *edge = GDK_WINDOW_EDGE_SOUTH_EAST;
    }
    return true;
  }

  NOTREACHED();
  return false;
}
