  void CallBoolMethod(dbus::MethodCall* method_call,
                      const BoolDBusMethodCallback& callback) {
    proxy_->CallMethod(method_call, dbus::ObjectProxy::TIMEOUT_USE_DEFAULT,
                       base::Bind(
                           &CryptohomeClientImpl::OnBoolMethod,
                           weak_ptr_factory_.GetWeakPtr(),
                           callback));
  }
