void BrowserWindowGtk::HideDevToolsContainer() {
  bool to_right = devtools_dock_side_ == DEVTOOLS_DOCK_SIDE_RIGHT;
  gtk_container_remove(GTK_CONTAINER(to_right ? contents_hsplit_ :
                           contents_vsplit_),
                       devtools_container_->widget());
}
