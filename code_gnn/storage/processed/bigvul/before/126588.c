void TabStripGtk::TabChangedAt(TabContents* contents, int index,
                               TabChangeType change_type) {
  TabGtk* tab = GetTabAtAdjustForAnimation(index);
  if (change_type == TITLE_NOT_LOADING) {
    if (tab->mini() && !tab->IsActive())
      tab->StartMiniTabTitleAnimation();
    return;
  }
  tab->UpdateData(contents->web_contents(),
                  model_->IsAppTab(index),
                  change_type == LOADING_ONLY);
  tab->UpdateFromModel();
}
