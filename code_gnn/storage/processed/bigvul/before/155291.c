void ChromeContentBrowserClient::BindInterfaceRequestFromFrame(
    content::RenderFrameHost* render_frame_host,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  if (!frame_interfaces_ && !frame_interfaces_parameterized_ &&
      !worker_interfaces_parameterized_) {
    InitWebContextInterfaces();
  }

  if (!frame_interfaces_parameterized_->TryBindInterface(
          interface_name, &interface_pipe, render_frame_host)) {
    frame_interfaces_->TryBindInterface(interface_name, &interface_pipe);
  }
}
