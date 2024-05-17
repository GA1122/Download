RootWindowController* Shell::GetPrimaryRootWindowController() {
  CHECK(HasInstance());
  return GetRootWindowController(GetPrimaryRootWindow());
}
