CreateDownloadURLLoaderFactoryGetter(StoragePartitionImpl* storage_partition,
                                     RenderFrameHost* rfh,
                                     bool is_download) {
  network::mojom::URLLoaderFactoryPtrInfo proxy_factory_ptr_info;
  network::mojom::URLLoaderFactoryRequest proxy_factory_request;
  if (rfh) {
    network::mojom::URLLoaderFactoryPtrInfo devtools_factory_ptr_info;
    network::mojom::URLLoaderFactoryRequest devtools_factory_request =
        MakeRequest(&devtools_factory_ptr_info);
    if (devtools_instrumentation::WillCreateURLLoaderFactory(
            static_cast<RenderFrameHostImpl*>(rfh), true, is_download,
            &devtools_factory_request)) {
      proxy_factory_ptr_info = std::move(devtools_factory_ptr_info);
      proxy_factory_request = std::move(devtools_factory_request);
    }
  }
  return base::MakeRefCounted<NetworkDownloadURLLoaderFactoryGetter>(
      storage_partition->url_loader_factory_getter(),
      std::move(proxy_factory_ptr_info), std::move(proxy_factory_request));
}
