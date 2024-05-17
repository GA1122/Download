void ShelfLayoutManager::SetLauncher(Launcher* launcher) {
  if (launcher == launcher_)
    return;

  launcher_ = launcher;
  LayoutShelf();
}
