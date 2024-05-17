CreateDownloadURLLoaderFactoryGetterFromURLLoaderFactory(
    std::unique_ptr<network::mojom::URLLoaderFactory> factory) {
  network::mojom::URLLoaderFactoryPtr factory_ptr;
  mojo::MakeStrongBinding(std::move(factory), mojo::MakeRequest(&factory_ptr));
  network::mojom::URLLoaderFactoryPtrInfo factory_ptr_info =
      factory_ptr.PassInterface();

  auto wrapper_factory =
      std::make_unique<network::WrapperSharedURLLoaderFactoryInfo>(
          std::move(factory_ptr_info));
  return base::MakeRefCounted<download::DownloadURLLoaderFactoryGetterImpl>(
      std::move(wrapper_factory));
}
