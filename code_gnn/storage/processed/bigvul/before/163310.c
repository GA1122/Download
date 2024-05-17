void CreateFrameFactory(mojom::FrameFactoryRequest request,
                        const service_manager::BindSourceInfo& source_info) {
  mojo::MakeStrongBinding(std::make_unique<FrameFactoryImpl>(source_info),
                          std::move(request));
}
