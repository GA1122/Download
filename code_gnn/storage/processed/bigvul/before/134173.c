HWND InputMethodWin::GetAttachedWindowHandle(
  const TextInputClient* text_input_client) const {
#if defined(USE_AURA)
  return toplevel_window_handle_;
#else
  if (!text_input_client)
    return NULL;
  return text_input_client->GetAttachedWindow();
#endif
}
