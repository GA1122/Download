void Dispatcher::OnDispatchOnConnect(
    int target_port_id,
    const std::string& channel_name,
    const ExtensionMsg_TabConnectionInfo& source,
    const ExtensionMsg_ExternalConnectionInfo& info,
    const std::string& tls_channel_id) {
  DCHECK(!ContainsKey(port_to_tab_id_map_, target_port_id));
  DCHECK_EQ(1, target_port_id % 2);   
  int sender_tab_id = -1;
  source.tab.GetInteger("id", &sender_tab_id);
  port_to_tab_id_map_[target_port_id] = sender_tab_id;

  MessagingBindings::DispatchOnConnect(*script_context_set_, target_port_id,
                                       channel_name, source, info,
                                       tls_channel_id,
                                       NULL);   
}
