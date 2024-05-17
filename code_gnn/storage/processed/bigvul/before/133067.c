static BOOL CALLBACK ClipDCToChild(HWND window, LPARAM param) {
  ClipState* clip_state = reinterpret_cast<ClipState*>(param);
  if (GetParent(window) == clip_state->parent && IsWindowVisible(window)) {
    RECT bounds;
    GetWindowRect(window, &bounds);
    ExcludeClipRect(clip_state->dc,
      bounds.left - clip_state->x,
      bounds.top - clip_state->y,
      bounds.right - clip_state->x,
      bounds.bottom - clip_state->y);
  }
  return TRUE;
}
