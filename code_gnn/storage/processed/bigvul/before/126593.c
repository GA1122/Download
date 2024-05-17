void TabStripGtk::TabSelectionChanged(TabStripModel* tab_strip_model,
                                      const TabStripSelectionModel& old_model) {
  bool tiny_tabs = current_unselected_width_ != current_selected_width_;
  if (!IsAnimating() && (!needs_resize_layout_ || tiny_tabs))
    Layout();

  if (model_->active_index() >= 0)
    GetTabAt(model_->active_index())->SchedulePaint();

  if (old_model.active() >= 0) {
    GetTabAt(old_model.active())->SchedulePaint();
    GetTabAt(old_model.active())->StopMiniTabTitleAnimation();
  }

  std::vector<int> indices_affected;
  std::insert_iterator<std::vector<int> > it1(indices_affected,
                                              indices_affected.begin());
  std::set_symmetric_difference(
      old_model.selected_indices().begin(),
      old_model.selected_indices().end(),
      model_->selection_model().selected_indices().begin(),
      model_->selection_model().selected_indices().end(),
      it1);
  for (std::vector<int>::iterator it = indices_affected.begin();
       it != indices_affected.end(); ++it) {
    if (*it != model_->active_index() && *it != old_model.active())
      GetTabAtAdjustForAnimation(*it)->SchedulePaint();
  }

  TabStripSelectionModel::SelectedIndices no_longer_selected;
  std::insert_iterator<std::vector<int> > it2(no_longer_selected,
                                              no_longer_selected.begin());
  std::set_difference(old_model.selected_indices().begin(),
                      old_model.selected_indices().end(),
                      model_->selection_model().selected_indices().begin(),
                      model_->selection_model().selected_indices().end(),
                      it2);
  for (std::vector<int>::iterator it = no_longer_selected.begin();
       it != no_longer_selected.end(); ++it) {
    GetTabAtAdjustForAnimation(*it)->StopMiniTabTitleAnimation();
  }
}
