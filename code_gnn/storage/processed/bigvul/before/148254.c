void PepperRendererConnection::OnMsgCreateResourceHostsFromHost(
    int routing_id,
    int child_process_id,
    const ppapi::proxy::ResourceMessageCallParams& params,
    PP_Instance instance,
    const std::vector<IPC::Message>& nested_msgs) {
  BrowserPpapiHostImpl* host = GetHostForChildProcess(child_process_id);
  if (!host) {
    DLOG(ERROR) << "Invalid plugin process ID.";
    return;
  }

  scoped_refptr<PendingHostCreator> creator = new PendingHostCreator(
      host, this, routing_id, params.sequence(), nested_msgs.size());
  for (size_t i = 0; i < nested_msgs.size(); ++i) {
    const IPC::Message& nested_msg = nested_msgs[i];
    std::unique_ptr<ppapi::host::ResourceHost> resource_host;
    if (host->IsValidInstance(instance)) {
      if (nested_msg.type() == PpapiHostMsg_FileRef_CreateForRawFS::ID) {
        base::FilePath external_path;
        if (ppapi::UnpackMessage<PpapiHostMsg_FileRef_CreateForRawFS>(
                nested_msg, &external_path)) {
          resource_host.reset(new PepperFileRefHost(
              host, instance, params.pp_resource(), external_path));
        }
      } else if (nested_msg.type() ==
                 PpapiHostMsg_FileSystem_CreateFromRenderer::ID) {
        std::string root_url;
        PP_FileSystemType file_system_type;
        if (ppapi::UnpackMessage<PpapiHostMsg_FileSystem_CreateFromRenderer>(
                nested_msg, &root_url, &file_system_type)) {
          PepperFileSystemBrowserHost* browser_host =
              new PepperFileSystemBrowserHost(
                  host, instance, params.pp_resource(), file_system_type);
          resource_host.reset(browser_host);
          browser_host->OpenExisting(
              GURL(root_url),
              base::Bind(&PendingHostCreator::AddPendingResourceHost,
                         creator,
                         i,
                         base::Passed(&resource_host)));
          continue;
        }
      }
    }

    if (!resource_host.get()) {
      resource_host = host->GetPpapiHost()->CreateResourceHost(
          params.pp_resource(), instance, nested_msg);
    }

    if (resource_host.get())
      creator->AddPendingResourceHost(i, std::move(resource_host));
  }

}
