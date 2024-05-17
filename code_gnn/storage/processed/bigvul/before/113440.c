InputMethodIBus::InputMethodIBus(
    internal::InputMethodDelegate* delegate)
    :
#if defined(HAVE_IBUS)
      ibus_client_(new internal::IBusClientImpl),
#else
      ibus_client_(new internal::MockIBusClient),
#endif
      context_(NULL),
      pending_create_ic_request_(NULL),
      context_focused_(false),
      composing_text_(false),
      composition_changed_(false),
      suppress_next_result_(false) {
  SetDelegate(delegate);
}
