gboolean TabContentsContainerGtk::OnFocus(GtkWidget* widget,
                                          GtkDirectionType focus) {
  if (preview_) {
    gtk_widget_child_focus(tab_->web_contents()->GetContentNativeView(), focus);
    return TRUE;
  }

  return FALSE;
}
