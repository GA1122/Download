  const aura::Window* GetSelectedWindow() {
    WindowSelector* ws = window_selector();
    WindowSelectorItem* item =
        ws->grid_list_[ws->selected_grid_index_]->SelectedWindow();
    if (!item)
      return nullptr;
    return item->GetWindow();
  }
