TabContents* Browser::AddTab(TabContentsWrapper* tab_contents,
                             PageTransition::Type type) {
  tab_handler_->GetTabStripModel()->AddTabContents(
      tab_contents, -1, type, TabStripModel::ADD_ACTIVE);
  return tab_contents->tab_contents();
}
