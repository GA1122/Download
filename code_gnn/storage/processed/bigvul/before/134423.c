void TabStrip::SetSelection(const ui::ListSelectionModel& old_selection,
                            const ui::ListSelectionModel& new_selection) {
  if (touch_layout_.get()) {
    touch_layout_->SetActiveIndex(new_selection.active());
    if (!views::ViewModelUtils::IsAtIdealBounds(tabs_))
      AnimateToIdealBounds();
    SchedulePaint();
  } else {
    bool tiny_tabs = current_unselected_width_ != current_selected_width_;
    if (!IsAnimating() && (!in_tab_close_ || tiny_tabs)) {
      DoLayout();
    } else {
      SchedulePaint();
    }
  }

  ui::ListSelectionModel::SelectedIndices no_longer_selected =
      base::STLSetDifference<ui::ListSelectionModel::SelectedIndices>(
          old_selection.selected_indices(),
          new_selection.selected_indices());
  for (size_t i = 0; i < no_longer_selected.size(); ++i)
    tab_at(no_longer_selected[i])->StopMiniTabTitleAnimation();
}
