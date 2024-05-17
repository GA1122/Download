void BrowserWindowGtk::ConnectAccelerators() {
  accel_group_ = gtk_accel_group_new();
  gtk_window_add_accel_group(window_, accel_group_);

  AcceleratorsGtk* accelerators = AcceleratorsGtk::GetInstance();
  for (AcceleratorsGtk::const_iterator iter = accelerators->begin();
       iter != accelerators->end(); ++iter) {
    gtk_accel_group_connect(
        accel_group_,
        iter->second.GetGdkKeyCode(),
        static_cast<GdkModifierType>(iter->second.modifiers()),
        GtkAccelFlags(0),
        g_cclosure_new(G_CALLBACK(OnGtkAccelerator),
                       GINT_TO_POINTER(iter->first), NULL));
  }
}
