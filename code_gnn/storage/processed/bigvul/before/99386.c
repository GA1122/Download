  virtual HRESULT WINAPI QueryInterface(REFIID riid, void**object) {
    if (riid == IID_IUnknown) {
      *object = ToIUnknown();
    } else if (riid == IID_IPrintDialogCallback) {
      *object = static_cast<IPrintDialogCallback*>(this);
    } else if (riid == IID_IObjectWithSite) {
      *object = static_cast<IObjectWithSite*>(this);
    } else {
      return E_NOINTERFACE;
    }
    return S_OK;
  }
