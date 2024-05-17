void ClipboardWin::Stop() {
  client_clipboard_.reset();

  if (window_ && remove_clipboard_format_listener_)
    (*remove_clipboard_format_listener_)(window_->hwnd());

  window_.reset();
}
