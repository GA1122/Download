TabContents* Browser::GetOrCloneTabForDisposition(
       WindowOpenDisposition disposition) {
  TabContentsWrapper* current_tab = GetSelectedTabContentsWrapper();
  switch (disposition) {
    case NEW_FOREGROUND_TAB:
    case NEW_BACKGROUND_TAB: {
      current_tab = current_tab->Clone();
      tab_handler_->GetTabStripModel()->AddTabContents(
          current_tab, -1, PageTransition::LINK,
          disposition == NEW_FOREGROUND_TAB ? TabStripModel::ADD_ACTIVE :
                                              TabStripModel::ADD_NONE);
      break;
    }
    case NEW_WINDOW: {
      current_tab = current_tab->Clone();
      Browser* browser = Browser::Create(profile_);
      browser->tabstrip_model()->AddTabContents(
          current_tab, -1, PageTransition::LINK, TabStripModel::ADD_ACTIVE);
      browser->window()->Show();
      break;
    }
    default:
      break;
  }
  return current_tab->tab_contents();
}
