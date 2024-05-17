  virtual WebDevToolsAgent* agent() {
    DevToolsAgent* agent = DevToolsAgent::FromHostId(host_id);
    if (!agent)
      return 0;
    return agent->GetWebAgent();
  }
