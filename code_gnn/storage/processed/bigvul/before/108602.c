void Shell::PlatformSetIsLoading(bool loading) {
  if (loading)
    gtk_spinner_start(GTK_SPINNER(spinner_));
  else
    gtk_spinner_stop(GTK_SPINNER(spinner_));
}
