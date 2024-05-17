  HANDLE GetData(UINT format) {
    if (!opened_) {
      NOTREACHED();
      return NULL;
    }
    return ::GetClipboardData(format);
  }
