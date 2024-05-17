bool IsXButtonUpEvent(const ui::MouseEvent* event) {
#if defined(OS_WIN)
  switch (event->native_event().message) {
    case WM_XBUTTONUP:
    case WM_NCXBUTTONUP:
      return true;
  }
#endif
  return false;
}
