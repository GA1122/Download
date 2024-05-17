void RenderFrameImpl::GetInterfaceProvider(
    service_manager::mojom::InterfaceProviderRequest request) {
  service_manager::Connector* connector = ChildThread::Get()->GetConnector();
  service_manager::mojom::InterfaceProviderPtr provider;
  interface_provider_bindings_.AddBinding(this, mojo::MakeRequest(&provider));
  connector->FilterInterfaces(mojom::kNavigation_FrameSpec,
                              browser_info_.identity, std::move(request),
                              std::move(provider));
}
