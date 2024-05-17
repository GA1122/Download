  BOOL Empty() {
    if (!opened_) {
      NOTREACHED();
      return false;
    }
    return ::EmptyClipboard();
  }
