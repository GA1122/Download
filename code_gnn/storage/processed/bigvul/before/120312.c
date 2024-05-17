  void OnAsyncCallStatusWithData(dbus::Signal* signal) {
    dbus::MessageReader reader(signal);
    int async_id = 0;
    bool return_status = false;
    const uint8* return_data_buffer = NULL;
    size_t return_data_length = 0;
    if (!reader.PopInt32(&async_id) ||
        !reader.PopBool(&return_status) ||
        !reader.PopArrayOfBytes(&return_data_buffer, &return_data_length)) {
      LOG(ERROR) << "Invalid signal: " << signal->ToString();
      return;
    }
    if (!async_call_status_data_handler_.is_null()) {
      std::string return_data(reinterpret_cast<const char*>(return_data_buffer),
                              return_data_length);
      async_call_status_data_handler_.Run(async_id, return_status, return_data);
    }
  }
