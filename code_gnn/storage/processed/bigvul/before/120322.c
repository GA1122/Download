  void OnVoidMethod(const VoidDBusMethodCallback& callback,
                    dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE);
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS);
  }
