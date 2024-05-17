void ChromeContentBrowserClient::BindInterfaceRequestFromWorker(
    content::RenderProcessHost* render_process_host,
    const url::Origin& origin,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  if (!frame_interfaces_ && !frame_interfaces_parameterized_ &&
      !worker_interfaces_parameterized_) {
    InitWebContextInterfaces();
  }

  worker_interfaces_parameterized_->BindInterface(
      interface_name, std::move(interface_pipe), render_process_host, origin);
}
