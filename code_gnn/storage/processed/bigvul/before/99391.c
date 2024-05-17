  IUnknown* ToIUnknown() {
    return static_cast<IUnknown*>(static_cast<IPrintDialogCallback*>(this));
  }
