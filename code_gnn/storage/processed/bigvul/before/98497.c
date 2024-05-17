void AeroPeekManager::TabInsertedAt(TabContents* contents,
                                    int index,
                                    bool foreground) {
  if (GetAeroPeekWindow(GetTabID(contents)))
    return;

  AeroPeekWindow* window = new AeroPeekWindow(application_window_,
                                              this,
                                              GetTabID(contents),
                                              foreground,
                                              contents->GetTitle(),
                                              contents->GetFavIcon());
  if (!window)
    return;

  tab_list_.push_back(window);
}
