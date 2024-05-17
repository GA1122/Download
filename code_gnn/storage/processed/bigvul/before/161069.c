  void OnMessage(const std::string& message) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    if (agent_host_.get())
      agent_host_->DispatchProtocolMessage(this, message);
  }
