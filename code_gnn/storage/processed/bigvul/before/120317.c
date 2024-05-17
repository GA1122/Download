  void OnGetSystemSalt(const GetSystemSaltCallback& callback,
                       dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::vector<uint8>());
      return;
    }
    dbus::MessageReader reader(response);
    const uint8* bytes = NULL;
    size_t length = 0;
    if (!reader.PopArrayOfBytes(&bytes, &length)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, std::vector<uint8>());
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS,
                 std::vector<uint8>(bytes, bytes + length));
  }
