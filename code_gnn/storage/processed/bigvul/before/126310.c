void BrowserWindowGtk::AddFindBar(FindBarGtk* findbar) {
  gtk_floating_container_add_floating(
      GTK_FLOATING_CONTAINER(render_area_floating_container_),
      findbar->widget());
}
