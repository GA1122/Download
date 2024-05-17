void AeroPeekManager::TabChangedAt(TabContents* contents,
                                   int index,
                                   TabChangeType change_type) {
  AeroPeekWindow* window = GetAeroPeekWindow(GetTabID(contents));
  if (!window)
    return;

  window->SetTitle(contents->GetTitle());
  window->SetFavIcon(contents->GetFavIcon());
  window->Update(contents->is_loading());
}
