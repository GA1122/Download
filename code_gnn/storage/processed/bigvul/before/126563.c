void TabStripGtk::RemoveTabAt(int index) {
  TabGtk* removed = tab_data_.at(index).tab;

  tab_data_.erase(tab_data_.begin() + index);

  if (!IsDragSessionActive() || !drag_controller_->IsDraggingTab(removed)) {
    gtk_container_remove(GTK_CONTAINER(tabstrip_.get()), removed->widget());
    delete removed;
  }
}
