void AeroPeekManager::TabClosingAt(TabContents* contents, int index) {
  AeroPeekWindow* window = GetAeroPeekWindow(GetTabID(contents));
  if (!window)
    return;

  window->Destroy();
  DeleteAeroPeekWindow(GetTabID(contents));
}
