void WindowTreeHostManager::CloseChildWindows() {
  for (WindowTreeHostMap::const_iterator it = window_tree_hosts_.begin();
       it != window_tree_hosts_.end(); ++it) {
    aura::Window* root_window = GetWindow(it->second);
    RootWindowController* controller = GetRootWindowController(root_window);
    if (controller) {
      controller->CloseChildWindows();
    } else {
      while (!root_window->children().empty()) {
        aura::Window* child = root_window->children()[0];
        delete child;
      }
    }
  }
}
