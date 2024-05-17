void RenderFrameHostImpl::InvalidateMojoConnection() {
  registry_.reset();

  frame_.reset();
  frame_bindings_control_.reset();
  frame_host_associated_binding_.Close();
  navigation_control_.reset();

  mojo_image_downloader_.reset();

  frame_resource_coordinator_.reset();

  geolocation_service_.reset();
  sensor_provider_proxy_.reset();
}
