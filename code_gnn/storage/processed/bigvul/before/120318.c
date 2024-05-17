  void OnPkcs11GetTpmTokenInfo(const Pkcs11GetTpmTokenInfoCallback& callback,
                               dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string(), std::string(), -1);
      return;
    }
    dbus::MessageReader reader(response);
    std::string label;
    std::string user_pin;
    if (!reader.PopString(&label) || !reader.PopString(&user_pin)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string(), std::string(), -1);
      LOG(ERROR) << "Invalid response: " << response->ToString();
      return;
    }
    const int kDefaultSlot = 0;
    callback.Run(DBUS_METHOD_CALL_SUCCESS, label, user_pin, kDefaultSlot);
  }
