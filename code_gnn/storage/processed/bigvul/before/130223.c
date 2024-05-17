void ClipboardWin::InjectClipboardEvent(
    const protocol::ClipboardEvent& event) {
  if (!window_)
    return;

  if (event.mime_type().compare(kMimeTypeTextUtf8) != 0)
    return;
  if (!StringIsUtf8(event.data().c_str(), event.data().length())) {
    LOG(ERROR) << "ClipboardEvent: data is not UTF-8 encoded.";
    return;
  }

  base::string16 text = base::UTF8ToUTF16(ReplaceLfByCrLf(event.data()));

  ScopedClipboard clipboard;
  if (!clipboard.Init(window_->hwnd())) {
    LOG(WARNING) << "Couldn't open the clipboard.";
    return;
  }

  clipboard.Empty();

  HGLOBAL text_global =
      ::GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(WCHAR));
  if (!text_global) {
    LOG(WARNING) << "Couldn't allocate global memory.";
    return;
  }

  LPWSTR text_global_locked =
      reinterpret_cast<LPWSTR>(::GlobalLock(text_global));
  memcpy(text_global_locked, text.data(), text.size() * sizeof(WCHAR));
  text_global_locked[text.size()] = (WCHAR)0;
  ::GlobalUnlock(text_global);

  clipboard.SetData(CF_UNICODETEXT, text_global);
}
