  void SendMessageToAgentHost(const std::string& message) {
    if (throttle_) {
      bool resuming = false;
      std::unique_ptr<base::Value> value = base::JSONReader::Read(message);
      if (value && value->is_dict()) {
        base::Value* method = value->FindKey(kMethod);
        resuming = method && method->is_string() &&
                   method->GetString() == kResumeMethod;
      }
      if (resuming)
        throttle_->Clear();
    }

    agent_host_->DispatchProtocolMessage(this, message);
  }
