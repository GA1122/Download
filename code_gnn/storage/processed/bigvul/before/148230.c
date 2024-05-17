BrowserPpapiHostImpl::BrowserPpapiHostImpl(
    IPC::Sender* sender,
    const ppapi::PpapiPermissions& permissions,
    const std::string& plugin_name,
    const base::FilePath& plugin_path,
    const base::FilePath& profile_data_directory,
    bool in_process,
    bool external_plugin)
    : ppapi_host_(new ppapi::host::PpapiHost(sender, permissions)),
      plugin_name_(plugin_name),
      plugin_path_(plugin_path),
      profile_data_directory_(profile_data_directory),
      in_process_(in_process),
      external_plugin_(external_plugin),
      ssl_context_helper_(new SSLContextHelper()) {
  message_filter_ = new HostMessageFilter(ppapi_host_.get(), this);
  ppapi_host_->AddHostFactoryFilter(std::unique_ptr<ppapi::host::HostFactory>(
      new ContentBrowserPepperHostFactory(this)));
}
