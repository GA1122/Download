void AutocompletePopupViewGtk::StackWindow() {
  gfx::NativeView edit_view = edit_view_->GetNativeView();
  DCHECK(GTK_IS_WIDGET(edit_view));
  GtkWidget* toplevel = gtk_widget_get_toplevel(edit_view);
  DCHECK(GTK_WIDGET_TOPLEVEL(toplevel));
  gtk_util::StackPopupWindow(window_, toplevel);
}
