network::mojom::NetworkContext* StoragePartitionImpl::GetNetworkContext() {
  if (!network_context_.is_bound())
    InitNetworkContext();
  return network_context_.get();
}
