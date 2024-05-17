InputMethodIBus::PendingCreateICRequestImpl::PendingCreateICRequestImpl(
    InputMethodIBus* input_method,
    internal::IBusClient* ibus_client,
    PendingCreateICRequestImpl** request_ptr)
    : input_method_(input_method),
      ibus_client_(ibus_client),
      request_ptr_(request_ptr) {
}
