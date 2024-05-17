RenderFrameHostImpl::GetJavaRenderFrameHost() {
  RenderFrameHostAndroid* render_frame_host_android =
      static_cast<RenderFrameHostAndroid*>(
          GetUserData(kRenderFrameHostAndroidKey));
  if (!render_frame_host_android) {
    java_interface_registry_ =
        base::MakeUnique<service_manager::InterfaceRegistry>(
            "RenderFrameHost Java");
    service_manager::mojom::InterfaceProviderPtr interface_provider_ptr;
    java_interface_registry_->set_default_binder(
        base::Bind(&RenderFrameHostImpl::ForwardGetInterfaceToRenderFrame,
                   weak_ptr_factory_.GetWeakPtr()));
    java_interface_registry_->Bind(
        mojo::MakeRequest(&interface_provider_ptr), service_manager::Identity(),
        service_manager::InterfaceProviderSpec(), service_manager::Identity(),
        service_manager::InterfaceProviderSpec());
    render_frame_host_android =
        new RenderFrameHostAndroid(this, std::move(interface_provider_ptr));
    SetUserData(kRenderFrameHostAndroidKey, render_frame_host_android);
  }
  return render_frame_host_android->GetJavaObject();
}
