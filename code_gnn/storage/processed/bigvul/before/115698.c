void HostNPScriptObject::ReadPolicyAndConnect(const std::string& uid,
                                              const std::string& auth_token,
                                              const std::string& auth_service) {
  if (MessageLoop::current() != host_context_.main_message_loop()) {
    host_context_.main_message_loop()->PostTask(
        FROM_HERE, base::Bind(
            &HostNPScriptObject::ReadPolicyAndConnect, base::Unretained(this),
            uid, auth_token, auth_service));
    return;
  }

  SetState(kStarting);

  if (policy_received_) {
    FinishConnect(uid, auth_token, auth_service);
  } else {
    pending_connect_ =
        base::Bind(&HostNPScriptObject::FinishConnect,
                   base::Unretained(this), uid, auth_token, auth_service);
  }
}
