void OnDialogResponse(GtkDialog* dialog, gint response_id,
                      PageInfoWindowGtk* page_info) {
  if (response_id == RESPONSE_SHOW_CERT_INFO) {
    page_info->ShowCertDialog();
  } else {
    gtk_widget_destroy(GTK_WIDGET(dialog));
  }
}
