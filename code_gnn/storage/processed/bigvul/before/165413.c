void StoragePartitionImpl::InitNetworkContext() {
  network_context_ = GetContentClient()->browser()->CreateNetworkContext(
      browser_context_, is_in_memory_, relative_partition_path_);
  if (!network_context_) {
    CHECK(url_request_context_);

    DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
    DCHECK(!network_context_owner_);
    network_context_owner_ = std::make_unique<NetworkContextOwner>();
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(&NetworkContextOwner::Initialize,
                       base::Unretained(network_context_owner_.get()),
                       MakeRequest(&network_context_), url_request_context_));
  }
  network::mojom::NetworkContextClientPtr client_ptr;
  network_context_client_binding_.Close();
  network_context_client_binding_.Bind(mojo::MakeRequest(&client_ptr));
  network_context_->SetClient(std::move(client_ptr));
  network_context_.set_connection_error_handler(base::BindOnce(
      &StoragePartitionImpl::InitNetworkContext, weak_factory_.GetWeakPtr()));
}
