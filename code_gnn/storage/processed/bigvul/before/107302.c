TabContents* Browser::GetOrCloneTabForDisposition(
       WindowOpenDisposition disposition) {
  TabContentsWrapper* current_tab = GetSelectedTabContentsWrapper();
  if (ShouldOpenNewTabForWindowDisposition(disposition)) {
    current_tab = current_tab->Clone();
    tab_handler_->GetTabStripModel()->AddTabContents(
        current_tab, -1, PageTransition::LINK,
        disposition == NEW_FOREGROUND_TAB ? TabStripModel::ADD_SELECTED :
                                            TabStripModel::ADD_NONE);
  }
  return current_tab->tab_contents();
}
