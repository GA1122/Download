void Shell::PlatformEnableUIControl(UIControl control, bool is_enabled) {
  GtkToolItem* item = NULL;
  switch (control) {
    case BACK_BUTTON:
      item = back_button_;
      break;
    case FORWARD_BUTTON:
      item = forward_button_;
      break;
    case STOP_BUTTON:
      item = stop_button_;
      break;
    default:
      NOTREACHED() << "Unknown UI control";
      return;
  }
  gtk_widget_set_sensitive(GTK_WIDGET(item), is_enabled);
}
