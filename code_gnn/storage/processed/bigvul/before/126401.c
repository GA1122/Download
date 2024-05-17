gboolean BrowserWindowGtk::OnGtkAccelerator(GtkAccelGroup* accel_group,
                                            GObject* acceleratable,
                                            guint keyval,
                                            GdkModifierType modifier,
                                            void* user_data) {
  int command_id = GPOINTER_TO_INT(user_data);
  BrowserWindowGtk* browser_window =
      GetBrowserWindowForNativeWindow(GTK_WINDOW(acceleratable));
  DCHECK(browser_window != NULL);
  return chrome::ExecuteCommand(browser_window->browser(), command_id);
}
