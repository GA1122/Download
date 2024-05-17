void TabStripModel::MoveSelectedTabsTo(int index) {
  int total_mini_count = IndexOfFirstNonMiniTab();
  int selected_mini_count = 0;
  int selected_count =
      static_cast<int>(selection_model_.selected_indices().size());
  for (int i = 0; i < selected_count &&
           IsMiniTab(selection_model_.selected_indices()[i]); ++i) {
    selected_mini_count++;
  }

  if (selected_mini_count > 0) {
    MoveSelectedTabsToImpl(
        std::min(total_mini_count - selected_mini_count, index), 0u,
        selected_mini_count);
    if (index > total_mini_count - selected_mini_count) {
      index += selected_mini_count;
    }
  }
  if (selected_mini_count == selected_count)
    return;

  MoveSelectedTabsToImpl(std::max(index, total_mini_count),
                         selected_mini_count,
                         selected_count - selected_mini_count);
}
