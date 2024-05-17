AutocompletePopupViewGtk::~AutocompletePopupViewGtk() {
  model_.reset();
  g_object_unref(layout_);
  gtk_widget_destroy(window_);
}
