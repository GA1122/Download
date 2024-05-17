gboolean BrowserWindowGtk::OnKeyPress(GtkWidget* widget, GdkEventKey* event) {
  if (extension_keybinding_registry_->HasPriorityHandler(event))
    return FALSE;

  WebContents* current_web_contents = chrome::GetActiveWebContents(browser());
  if (!current_web_contents || !current_web_contents->GetContentNativeView() ||
      !gtk_widget_is_focus(current_web_contents->GetContentNativeView())) {
    int command_id = GetCustomCommandId(event);
    if (command_id == -1)
      command_id = GetPreHandleCommandId(event);

    if (command_id != -1 && chrome::ExecuteCommand(browser_.get(), command_id))
      return TRUE;

    if (!gtk_window_propagate_key_event(GTK_WINDOW(widget), event)) {
      if (!gtk_window_activate_key(GTK_WINDOW(widget), event)) {
        gtk_bindings_activate_event(GTK_OBJECT(widget), event);
      }
    }
  } else {
    bool rv = gtk_window_propagate_key_event(GTK_WINDOW(widget), event);
    DCHECK(rv);
  }

  return TRUE;
}
