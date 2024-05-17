bool ShelfLayoutManager::IsShelfHiddenForFullscreen() const {
  const aura::Window* fullscreen_window =
      wm::GetWindowForFullscreenMode(shelf_widget_->GetNativeWindow());
  return fullscreen_window &&
         wm::GetWindowState(fullscreen_window)->GetHideShelfWhenFullscreen();
}
