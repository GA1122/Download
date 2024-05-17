void Dispatcher::OnDispatchOnDisconnect(int port_id,
                                        const std::string& error_message) {
  MessagingBindings::DispatchOnDisconnect(*script_context_set_, port_id,
                                          error_message,
                                          NULL);   
}
