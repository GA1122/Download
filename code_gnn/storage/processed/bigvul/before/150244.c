bool IsTopWindow(aura::Window* window) {
  DCHECK(window);
  return window == TabletModeWindowManager::GetTopWindow();
}
