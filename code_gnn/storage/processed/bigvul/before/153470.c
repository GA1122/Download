void TabStrip::SetSelection(const ui::ListSelectionModel& new_selection) {
  if (selected_tabs_.active() != new_selection.active()) {
    if (selected_tabs_.active() >= 0)
      tab_at(selected_tabs_.active())->ActiveStateChanged();
    if (new_selection.active() >= 0)
      tab_at(new_selection.active())->ActiveStateChanged();
  }

  if (touch_layout_) {
    touch_layout_->SetActiveIndex(new_selection.active());
    if (!views::ViewModelUtils::IsAtIdealBounds(tabs_))
      AnimateToIdealBounds();
    SchedulePaint();
  } else {
    if (GetActiveTabWidth() == GetInactiveTabWidth()) {
      SchedulePaint();
    } else if (IsAnimating()) {
      UpdateIdealBounds();
      AnimateToIdealBounds();
    } else {
      DoLayout();
    }
  }

  ui::ListSelectionModel::SelectedIndices no_longer_selected =
      base::STLSetDifference<ui::ListSelectionModel::SelectedIndices>(
          selected_tabs_.selected_indices(), new_selection.selected_indices());
  ui::ListSelectionModel::SelectedIndices newly_selected =
      base::STLSetDifference<ui::ListSelectionModel::SelectedIndices>(
          new_selection.selected_indices(), selected_tabs_.selected_indices());

  tab_at(new_selection.active())
      ->NotifyAccessibilityEvent(ax::mojom::Event::kSelection, true);
  selected_tabs_ = new_selection;

  UpdateHoverCard(nullptr, false);

  for (auto tab_index :
       base::STLSetUnion<ui::ListSelectionModel::SelectedIndices>(
           no_longer_selected, newly_selected)) {
    tab_at(tab_index)->SelectedStateChanged();
  }
}
