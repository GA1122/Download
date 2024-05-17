void TabStripModel::SelectLastTab() {
  ActivateTabAt(count() - 1, true);
}
