  void OnBoolMethod(const BoolDBusMethodCallback& callback,
                    dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false);
      return;
    }
    dbus::MessageReader reader(response);
    bool result = false;
    if (!reader.PopBool(&result)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false);
      LOG(ERROR) << "Invalid response: " << response->ToString();
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS, result);
  }
