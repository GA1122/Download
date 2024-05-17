void HandleFocusShelf() {
  base::RecordAction(UserMetricsAction("Accel_Focus_Shelf"));
  WmShell::Get()->focus_cycler()->FocusWidget(
      Shelf::ForPrimaryDisplay()->shelf_widget());
}
