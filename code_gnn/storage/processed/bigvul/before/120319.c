  void OnPkcs11GetTpmTokenInfoForUser(
      const Pkcs11GetTpmTokenInfoCallback& callback,
      dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string(), std::string(), -1);
      return;
    }
    dbus::MessageReader reader(response);
    std::string label;
    std::string user_pin;
    int slot = 0;
    if (!reader.PopString(&label) || !reader.PopString(&user_pin) ||
        !reader.PopInt32(&slot)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string(), std::string(), -1);
      LOG(ERROR) << "Invalid response: " << response->ToString();
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS, label, user_pin, slot);
  }
