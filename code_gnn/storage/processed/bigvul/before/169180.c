void RenderFrameHostImpl::SetUpMojoIfNeeded() {
  if (registry_.get())
    return;

  associated_registry_ = std::make_unique<AssociatedInterfaceRegistryImpl>();
  registry_ = std::make_unique<service_manager::BinderRegistry>();

  auto make_binding = [](RenderFrameHostImpl* impl,
                         mojom::FrameHostAssociatedRequest request) {
    impl->frame_host_associated_binding_.Bind(std::move(request));
  };
  static_cast<blink::AssociatedInterfaceRegistry*>(associated_registry_.get())
      ->AddInterface(base::Bind(make_binding, base::Unretained(this)));

  RegisterMojoInterfaces();
  mojom::FrameFactoryPtr frame_factory;
  BindInterface(GetProcess(), &frame_factory);
  frame_factory->CreateFrame(routing_id_, MakeRequest(&frame_));

  service_manager::mojom::InterfaceProviderPtr remote_interfaces;
  frame_->GetInterfaceProvider(mojo::MakeRequest(&remote_interfaces));
  remote_interfaces_.reset(new service_manager::InterfaceProvider);
  remote_interfaces_->Bind(std::move(remote_interfaces));

  remote_interfaces_->GetInterface(&frame_input_handler_);
}
