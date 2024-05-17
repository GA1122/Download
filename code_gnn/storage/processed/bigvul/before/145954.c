aura::Window* GetModalContainer() {
  return Shell::GetPrimaryRootWindowController()->GetContainer(
      ash::kShellWindowId_SystemModalContainer);
}
