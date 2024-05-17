void RenderFrameHostImpl::InvalidateMojoConnection() {
  interface_registry_.reset();

  ServiceManagerConnection* service_manager_connection =
      BrowserContext::GetServiceManagerConnectionFor(
          GetProcess()->GetBrowserContext());
  if (service_manager_connection) {
    service_manager_connection->RemoveOnConnectHandler(on_connect_handler_id_);
    on_connect_handler_id_ = 0;
  }

  frame_.reset();
  frame_host_interface_broker_binding_.Close();
  frame_bindings_control_.reset();

  mojo_image_downloader_.reset();
}
