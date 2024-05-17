gboolean TabStripGtk::OnDragLeave(GtkWidget* widget, GdkDragContext* context,
                                  guint time) {
  drop_info_->DestroyContainer();

  hover_tab_selector_.CancelTabTransition();

  return FALSE;
}
