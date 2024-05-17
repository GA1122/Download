void TabStripGtk::MaybeStartDrag(TabGtk* tab, const gfx::Point& point) {
  if (IsAnimating() || tab->closing() || !HasAvailableDragActions())
    return;

  std::vector<TabGtk*> tabs;
  for (size_t i = 0; i < model()->selection_model().size(); i++) {
    TabGtk* selected_tab =
        GetTabAtAdjustForAnimation(
            model()->selection_model().selected_indices()[i]);
    if (!selected_tab->closing())
      tabs.push_back(selected_tab);
  }

  drag_controller_.reset(new DraggedTabControllerGtk(this, tab, tabs));
  drag_controller_->CaptureDragInfo(point);
}
