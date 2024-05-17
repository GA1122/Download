void ChromeContentBrowserClient::BindInterfaceRequest(
    const service_manager::BindSourceInfo& source_info,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle* interface_pipe) {
  if (source_info.identity.name() == content::mojom::kGpuServiceName)
    gpu_binder_registry_.TryBindInterface(interface_name, interface_pipe);
}
