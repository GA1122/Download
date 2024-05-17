 bool DevToolsAgentHostImpl::DetachClient(DevToolsAgentHostClient* client) {
  if (!SessionByClient(client))
    return false;

  scoped_refptr<DevToolsAgentHostImpl> protect(this);
  InnerDetachClient(client);
  return true;
}
