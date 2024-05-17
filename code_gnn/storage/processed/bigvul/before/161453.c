  static std::string Attach(TargetHandler* handler,
                            DevToolsAgentHost* agent_host,
                            bool waiting_for_debugger) {
    std::string id = base::StringPrintf("%s:%d", agent_host->GetId().c_str(),
                                        ++handler->last_session_id_);
    Session* session = new Session(handler, agent_host, id);
    handler->attached_sessions_[id].reset(session);
    agent_host->AttachClient(session);
    handler->frontend_->AttachedToTarget(id, CreateInfo(agent_host),
                                         waiting_for_debugger);
    return id;
  }
