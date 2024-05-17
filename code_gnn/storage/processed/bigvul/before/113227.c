bool ShelfLayoutManager::IsShelfWindow(aura::Window* window) {
  if (!window)
    return false;
  return (launcher_widget() &&
          launcher_widget()->GetNativeWindow()->Contains(window)) ||
      (status_ && status_->GetNativeWindow()->Contains(window));
}
