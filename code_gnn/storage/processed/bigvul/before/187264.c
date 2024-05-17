  void DevToolsAgentHostImpl::AttachClient(DevToolsAgentHostClient* client) {
    if (SessionByClient(client))
      return;
  InnerAttachClient(client);
//   InnerAttachClient(client, false  );
// }
// 
// bool DevToolsAgentHostImpl::AttachRestrictedClient(
//     DevToolsAgentHostClient* client) {
//   if (SessionByClient(client))
//     return false;
//   return InnerAttachClient(client, true  );
  }