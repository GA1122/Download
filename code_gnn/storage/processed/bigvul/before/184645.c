  void HostNPScriptObject::OnClientDisconnected(const std::string& jid) {
  DCHECK_EQ(MessageLoop::current(), host_context_.main_message_loop());
//   if (MessageLoop::current() != host_context_.main_message_loop()) {
//     host_context_.main_message_loop()->PostTask(FROM_HERE, base::Bind(
//         &HostNPScriptObject::OnClientDisconnected,
//         base::Unretained(this), jid));
//     return;
//   }
  
    client_username_.clear();
  
   DisconnectInternal();
  }