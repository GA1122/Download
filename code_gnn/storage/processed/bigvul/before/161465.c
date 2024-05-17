  void Detach(bool host_closed) {
    handler_->frontend_->DetachedFromTarget(id_, agent_host_->GetId());
    if (host_closed)
      handler_->auto_attacher_.AgentHostClosed(agent_host_.get());
    else
      agent_host_->DetachClient(this);
    handler_->auto_attached_sessions_.erase(agent_host_.get());
    agent_host_ = nullptr;
    handler_->attached_sessions_.erase(id_);
  }
