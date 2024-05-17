void RenderFrameHostImpl::CreateNetworkServiceDefaultFactoryAndObserve(
    network::mojom::URLLoaderFactoryRequest default_factory_request) {
  StoragePartitionImpl* storage_partition =
      static_cast<StoragePartitionImpl*>(BrowserContext::GetStoragePartition(
          GetSiteInstance()->GetBrowserContext(), GetSiteInstance()));
  if (g_url_loader_factory_callback_for_test.Get().is_null()) {
    storage_partition->GetNetworkContext()->CreateURLLoaderFactory(
        std::move(default_factory_request), GetProcess()->GetID());
  } else {
    network::mojom::URLLoaderFactoryPtr original_factory;
    storage_partition->GetNetworkContext()->CreateURLLoaderFactory(
        mojo::MakeRequest(&original_factory), GetProcess()->GetID());
    g_url_loader_factory_callback_for_test.Get().Run(
        std::move(default_factory_request), GetProcess()->GetID(),
        original_factory.PassInterface());
  }

  if (IsOutOfProcessNetworkService()) {
    storage_partition->GetNetworkContext()->CreateURLLoaderFactory(
        mojo::MakeRequest(&network_service_connection_error_handler_holder_),
        GetProcess()->GetID());
    network_service_connection_error_handler_holder_
        .set_connection_error_handler(base::BindOnce(
            &RenderFrameHostImpl::OnNetworkServiceConnectionError,
            weak_ptr_factory_.GetWeakPtr()));
  }
}
