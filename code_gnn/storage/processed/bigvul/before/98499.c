void AeroPeekManager::TabSelectedAt(TabContents* old_contents,
                                    TabContents* new_contents,
                                    int index,
                                    bool user_gesture) {
  AeroPeekWindow* old_window = GetAeroPeekWindow(GetTabID(old_contents));
  if (old_window)
    old_window->Deactivate();

  AeroPeekWindow* new_window = GetAeroPeekWindow(GetTabID(new_contents));
  if (new_window)
    new_window->Activate();
}
