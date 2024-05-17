void Browser::SetTabContentBlocked(TabContents* contents, bool blocked) {
  int index = tabstrip_model()->GetWrapperIndex(contents);
  if (index == TabStripModel::kNoTab) {
    NOTREACHED();
    return;
  }
  tabstrip_model()->SetTabBlocked(index, blocked);
}
