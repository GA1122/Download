  const std::vector<std::unique_ptr<WindowSelectorItem>>& GetWindowItemsForRoot(
      int index) {
    return window_selector()->grid_list_[index]->window_list();
  }
