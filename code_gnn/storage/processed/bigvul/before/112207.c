aura::Window* GetModalContainer() {
  return Shell::GetInstance()->GetContainer(
      ash::internal::kShellWindowId_SystemModalContainer);
}
