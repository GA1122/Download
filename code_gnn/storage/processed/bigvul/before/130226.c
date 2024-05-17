void ClipboardWin::Start(
    scoped_ptr<protocol::ClipboardStub> client_clipboard) {
  DCHECK(!add_clipboard_format_listener_);
  DCHECK(!remove_clipboard_format_listener_);
  DCHECK(!window_);

  client_clipboard_.swap(client_clipboard);

  HMODULE user32 = GetModuleHandle(L"user32.dll");
  CHECK(user32);

  add_clipboard_format_listener_ =
      reinterpret_cast<AddClipboardFormatListenerFn*>(
          GetProcAddress(user32, "AddClipboardFormatListener"));
  if (add_clipboard_format_listener_) {
    remove_clipboard_format_listener_ =
        reinterpret_cast<RemoveClipboardFormatListenerFn*>(
            GetProcAddress(user32, "RemoveClipboardFormatListener"));
    CHECK(remove_clipboard_format_listener_);
  } else {
    LOG(WARNING) << "AddClipboardFormatListener() is not available.";
  }

  window_.reset(new base::win::MessageWindow());
  if (!window_->Create(base::Bind(&ClipboardWin::HandleMessage,
                                  base::Unretained(this)))) {
    LOG(ERROR) << "Couldn't create clipboard window.";
    window_.reset();
    return;
  }

  if (add_clipboard_format_listener_) {
    if (!(*add_clipboard_format_listener_)(window_->hwnd())) {
      LOG(WARNING) << "AddClipboardFormatListener() failed: " << GetLastError();
    }
  }
}
