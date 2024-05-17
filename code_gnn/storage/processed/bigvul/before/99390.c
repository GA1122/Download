  virtual HRESULT WINAPI SetSite(IUnknown* site) {
    if (!site) {
      DCHECK(services_);
      services_->Release();
      services_ = NULL;
      owner_.dialog_box_ = NULL;
    } else {
      DCHECK(services_ == NULL);
      HRESULT hr = site->QueryInterface(IID_IPrintDialogServices,
                                        reinterpret_cast<void**>(&services_));
      DCHECK(SUCCEEDED(hr));
    }
    return S_OK;
  }
