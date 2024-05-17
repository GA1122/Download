  void OnBaseReplyMethod(const ProtobufMethodCallback& callback,
                         dbus::Response* response) {
    cryptohome::BaseReply reply;
    if (!response) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false, reply);
      return;
    }
    dbus::MessageReader reader(response);
    if (!reader.PopArrayOfBytesAsProto(&reply)) {
      callback.Run(DBUS_METHOD_CALL_FAILURE, false, reply);
      return;
    }
    callback.Run(DBUS_METHOD_CALL_SUCCESS, true, reply);
  }
