void TabStripModel::MoveSelectedTabsToImpl(int index,
                                           size_t start,
                                           size_t length) {
  DCHECK(start < selection_model_.selected_indices().size() &&
         start + length <= selection_model_.selected_indices().size());
  size_t end = start + length;
  int count_before_index = 0;
  for (size_t i = start; i < end &&
       selection_model_.selected_indices()[i] < index + count_before_index;
       ++i) {
    count_before_index++;
  }

  int target_index = index + count_before_index;
  size_t tab_index = start;
  while (tab_index < end &&
         selection_model_.selected_indices()[start] < index) {
    MoveTabContentsAt(selection_model_.selected_indices()[start],
                      target_index - 1, false);
    tab_index++;
  }

  while (tab_index < end) {
    if (selection_model_.selected_indices()[tab_index] != target_index) {
      MoveTabContentsAt(selection_model_.selected_indices()[tab_index],
                        target_index, false);
    }
    tab_index++;
    target_index++;
  }
}
