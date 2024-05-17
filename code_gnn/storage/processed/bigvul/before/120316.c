  void OnDataMethod(const DataMethodCallback& callback,
                    dbus::Response* response) {
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false, std::string());
      return;
    }
    dbus::MessageReader reader(response);
    const uint8* data_buffer = NULL;
    size_t data_length = 0;
    bool result = false;
    if (!reader.PopArrayOfBytes(&data_buffer, &data_length) ||
        !reader.PopBool(&result)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false, std::string());
      return;
    }
    std::string data(reinterpret_cast<const char*>(data_buffer), data_length);
    callback.Run(DBUS_METHOD_CALL_SUCCESS, result, data);
  }
