  virtual HRESULT WINAPI HandleMessage(HWND dialog,
                                       UINT message,
                                       WPARAM wparam,
                                       LPARAM lparam,
                                       LRESULT* result) {
    if (!owner_.dialog_box_) {
      owner_.dialog_box_ = GetAncestor(dialog, GA_ROOT);
      EnableWindow(owner_hwnd_, TRUE);
    }
    return S_FALSE;
  }
