  bool CallBoolMethodAndBlock(dbus::MethodCall* method_call,
                              bool* result) {
    scoped_ptr<dbus::Response> response(
        blocking_method_caller_->CallMethodAndBlock(method_call));
    if (!response.get())
      return false;
    dbus::MessageReader reader(response.get());
    return reader.PopBool(result);
  }
