void RenderFrameHostImpl::BindInterfaceProviderRequest(
    service_manager::mojom::InterfaceProviderRequest
        interface_provider_request) {
  DCHECK(!document_scoped_interface_provider_binding_.is_bound());
  DCHECK(interface_provider_request.is_pending());
  document_scoped_interface_provider_binding_.Bind(
      FilterRendererExposedInterfaces(mojom::kNavigation_FrameSpec,
                                      GetProcess()->GetID(),
                                      std::move(interface_provider_request)));
}
