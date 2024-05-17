void InputMethodIBus::set_ibus_client(
    scoped_ptr<internal::IBusClient> new_client) {
  ibus_client_.swap(new_client);
}
