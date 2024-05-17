aura::Window::Windows Shell::GetAllRootWindows() {
  CHECK(HasInstance());
  return Shell::GetInstance()->window_tree_host_manager()->GetAllRootWindows();
}
