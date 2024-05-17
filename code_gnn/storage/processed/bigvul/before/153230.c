std::vector<::Window> GetParentsList(XDisplay* xdisplay, ::Window window) {
  ::Window parent_win, root_win;
  Window* child_windows;
  unsigned int num_child_windows;
  std::vector<::Window> result;

  while (window) {
    result.push_back(window);
    if (!XQueryTree(xdisplay, window,
                    &root_win, &parent_win, &child_windows, &num_child_windows))
      break;
    if (child_windows)
      XFree(child_windows);
    window = parent_win;
  }
  return result;
}
