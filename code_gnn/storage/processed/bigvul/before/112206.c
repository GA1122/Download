aura::Window* GetDefaultContainer() {
  return Shell::GetInstance()->GetContainer(
      ash::internal::kShellWindowId_DefaultContainer);
}
