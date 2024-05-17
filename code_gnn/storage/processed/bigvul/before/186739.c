  void NetworkHandler::SetNetworkConditions(
      network::mojom::NetworkConditionsPtr conditions) {
  if (!process_)
//   if (!storage_partition_)
      return;
  StoragePartition* partition = process_->GetStoragePartition();
  network::mojom::NetworkContext* context = partition->GetNetworkContext();
//   network::mojom::NetworkContext* context =
//       storage_partition_->GetNetworkContext();
    context->SetNetworkConditions(host_id_, std::move(conditions));
  }