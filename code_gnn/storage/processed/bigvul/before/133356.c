WindowTreeHostManager::GetAllRootWindowControllers() {
  std::vector<RootWindowController*> controllers;
  for (WindowTreeHostMap::const_iterator it = window_tree_hosts_.begin();
       it != window_tree_hosts_.end(); ++it) {
    RootWindowController* controller =
        GetRootWindowController(GetWindow(it->second));
    if (controller)
      controllers.push_back(controller);
  }
  return controllers;
}
