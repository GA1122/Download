  void CallVoidMethod(dbus::MethodCall* method_call,
                      const VoidDBusMethodCallback& callback) {
    proxy_->CallMethod(method_call, dbus::ObjectProxy::TIMEOUT_USE_DEFAULT,
                       base::Bind(&CryptohomeClientImpl::OnVoidMethod,
                                  weak_ptr_factory_.GetWeakPtr(),
                                  callback));
  }
