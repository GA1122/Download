void AutomationProvider::GetWindowTitle(int handle, string16* text) {
  gfx::NativeWindow window = window_tracker_->GetResource(handle);
  const gchar* title = gtk_window_get_title(window);
  text->assign(UTF8ToUTF16(title));
}
