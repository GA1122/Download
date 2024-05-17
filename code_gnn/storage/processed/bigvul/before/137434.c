std::unique_ptr<MessagePump> MessageLoop::CreateMessagePumpForType(Type type) {
#if defined(USE_GLIB) && !defined(OS_NACL)
  typedef MessagePumpGlib MessagePumpForUI;
#elif (defined(OS_LINUX) && !defined(OS_NACL)) || defined(OS_BSD)
  typedef MessagePumpLibevent MessagePumpForUI;
#elif defined(OS_FUCHSIA)
  typedef MessagePumpFuchsia MessagePumpForUI;
#endif

#if defined(OS_IOS) || defined(OS_MACOSX)
#define MESSAGE_PUMP_UI std::unique_ptr<MessagePump>(MessagePumpMac::Create())
#elif defined(OS_NACL) || defined(OS_AIX)
#define MESSAGE_PUMP_UI std::unique_ptr<MessagePump>()
#else
#define MESSAGE_PUMP_UI std::unique_ptr<MessagePump>(new MessagePumpForUI())
#endif

#if defined(OS_MACOSX)
#define MESSAGE_PUMP_DEFAULT \
  std::unique_ptr<MessagePump>(new MessagePumpCFRunLoop())
#else
#define MESSAGE_PUMP_DEFAULT \
  std::unique_ptr<MessagePump>(new MessagePumpDefault())
#endif

  if (type == MessageLoop::TYPE_UI) {
    if (message_pump_for_ui_factory_)
      return message_pump_for_ui_factory_();
    return MESSAGE_PUMP_UI;
  }
  if (type == MessageLoop::TYPE_IO)
    return std::unique_ptr<MessagePump>(new MessagePumpForIO());

#if defined(OS_ANDROID)
  if (type == MessageLoop::TYPE_JAVA)
    return std::unique_ptr<MessagePump>(new MessagePumpForUI());
#endif

  DCHECK_EQ(MessageLoop::TYPE_DEFAULT, type);
  return MESSAGE_PUMP_DEFAULT;
}
