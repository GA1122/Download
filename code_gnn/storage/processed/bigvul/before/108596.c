gboolean Shell::OnWindowDestroyed(GtkWidget* window) {
  delete this;
  return FALSE;   
}
