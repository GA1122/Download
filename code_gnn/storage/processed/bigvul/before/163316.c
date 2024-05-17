RenderThreadImpl::CreateUkmRecorderFactory() {
  ukm::mojom::UkmRecorderInterfacePtrInfo info;
  mojo::MakeRequest(&info);
  return std::make_unique<UkmRecorderFactoryImpl>(std::move(info));
}
