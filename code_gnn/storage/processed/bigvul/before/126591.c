void TabStripGtk::TabMoved(TabContents* contents,
                           int from_index,
                           int to_index) {
  gfx::Rect start_bounds = GetIdealBounds(from_index);
  TabGtk* tab = GetTabAt(from_index);
  tab_data_.erase(tab_data_.begin() + from_index);
  TabData data = {tab, gfx::Rect()};
  tab->set_mini(model_->IsMiniTab(to_index));
  tab->SetBlocked(model_->IsTabBlocked(to_index));
  tab_data_.insert(tab_data_.begin() + to_index, data);
  GenerateIdealBounds();
  StartMoveTabAnimation(from_index, to_index);
  ReStack();
}
