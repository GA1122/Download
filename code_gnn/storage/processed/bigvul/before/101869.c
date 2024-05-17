void Browser::UpdateTabStripModelInsertionPolicy() {
  tab_handler_->GetTabStripModel()->SetInsertionPolicy(UseVerticalTabs() ?
      TabStripModel::INSERT_BEFORE : TabStripModel::INSERT_AFTER);
}
