Shell::RootWindowControllerList Shell::GetAllRootWindowControllers() {
  CHECK(HasInstance());
  return Shell::GetInstance()
      ->window_tree_host_manager()
      ->GetAllRootWindowControllers();
}
