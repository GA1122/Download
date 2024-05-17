  void SetData(UINT uFormat, HANDLE hMem) {
    if (!opened_) {
      NOTREACHED();
      return;
    }
    ::SetClipboardData(uFormat, hMem);
  }
