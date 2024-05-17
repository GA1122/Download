void RenderFrameHostImpl::GetInterfaceProvider(
    service_manager::mojom::InterfaceProviderRequest interfaces) {
  service_manager::InterfaceProviderSpec browser_spec, renderer_spec;
  service_manager::GetInterfaceProviderSpec(
      mojom::kNavigation_FrameSpec, browser_info_.interface_provider_specs,
      &browser_spec);
  service_manager::GetInterfaceProviderSpec(
      mojom::kNavigation_FrameSpec, renderer_info_.interface_provider_specs,
      &renderer_spec);
  interface_registry_->Bind(std::move(interfaces),
                            browser_info_.identity, browser_spec,
                            renderer_info_.identity, renderer_spec);
}
