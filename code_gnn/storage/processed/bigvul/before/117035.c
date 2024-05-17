void OmniboxPopupViewGtk::StackWindow() {
  gfx::NativeView omnibox_view = omnibox_view_->GetNativeView();
  DCHECK(GTK_IS_WIDGET(omnibox_view));
  GtkWidget* toplevel = gtk_widget_get_toplevel(omnibox_view);
  DCHECK(gtk_widget_is_toplevel(toplevel));
  ui::StackPopupWindow(window_, toplevel);
}
