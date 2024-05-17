void TabStripModel::CloseSelectedTabs() {
  InternalCloseTabs(selection_model_.selected_indices(),
                    CLOSE_CREATE_HISTORICAL_TAB | CLOSE_USER_GESTURE);
}
