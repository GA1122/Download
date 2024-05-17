static XID GetTopLevelWindow(XID window) {
  bool parent_is_root;
  XID parent_window;

  if (!x11_util::GetWindowParent(&parent_window, &parent_is_root, window))
    return 0;
  if (parent_is_root)
    return window;

  return GetTopLevelWindow(parent_window);
}
