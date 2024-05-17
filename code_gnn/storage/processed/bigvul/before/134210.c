bool IsMouseEventFromTouch(UINT message) {
  return (message >= WM_MOUSEFIRST) && (message <= WM_MOUSELAST) &&
      (GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) ==
      MOUSEEVENTF_FROMTOUCH;
}
