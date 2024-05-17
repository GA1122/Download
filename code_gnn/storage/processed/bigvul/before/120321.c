  void OnStringMethod(const StringDBusMethodCallback& callback,
                      dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string());
      return;
    }
    dbus::MessageReader reader(response);
    std::string result;
    if (!reader.PopString(&result)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::string());
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS, result);
  }
