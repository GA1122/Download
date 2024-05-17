void BrowserWindowGtk::Close() {
  if (!window_)
    return;

  if (!CanClose())
    return;

  tabstrip_->StopAnimation();

  SaveWindowPosition();

  if (accel_group_) {
    AcceleratorsGtk* accelerators = AcceleratorsGtk::GetInstance();
    for (AcceleratorsGtk::const_iterator iter = accelerators->begin();
         iter != accelerators->end(); ++iter) {
      gtk_accel_group_disconnect_key(accel_group_,
          iter->second.GetGdkKeyCode(),
          static_cast<GdkModifierType>(iter->second.modifiers()));
    }
    gtk_window_remove_accel_group(window_, accel_group_);
    g_object_unref(accel_group_);
    accel_group_ = NULL;
  }

  window_configure_debounce_timer_.Stop();

  loading_animation_timer_.Stop();

  GtkWidget* window = GTK_WIDGET(window_);
  window_ = NULL;
  window_has_shown_ = false;
  titlebar_->set_window(NULL);

  global_menu_bar_->Disable();
  gtk_widget_destroy(window);
}
