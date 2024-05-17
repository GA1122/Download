std::unique_ptr<base::MessagePump> CreateMessagePumpForUI() {
  return std::unique_ptr<base::MessagePump>(new NestedMessagePumpAndroid());
};
