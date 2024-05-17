  bool selection_widget_active() {
    WindowSelector* ws = window_selector();
    return ws->grid_list_[ws->selected_grid_index_]->is_selecting();
  }
