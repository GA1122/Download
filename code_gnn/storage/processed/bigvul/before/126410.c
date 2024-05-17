void BrowserWindowGtk::PlaceBookmarkBar(bool is_floating) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::PlaceBookmarkBar");

  GtkWidget* target_parent = NULL;
  if (!is_floating) {
    target_parent = window_vbox_;
  } else {
    target_parent = render_area_vbox_;
  }

  GtkWidget* parent = gtk_widget_get_parent(bookmark_bar_->widget());
  if (parent != target_parent) {
    if (parent)
      gtk_container_remove(GTK_CONTAINER(parent), bookmark_bar_->widget());

    gtk_box_pack_end(GTK_BOX(target_parent), bookmark_bar_->widget(),
                     FALSE, FALSE, 0);
  }
}
