void PendingHostCreator::AddPendingResourceHost(
    size_t index,
    std::unique_ptr<ppapi::host::ResourceHost> resource_host) {
  pending_resource_host_ids_[index] =
      host_->GetPpapiHost()->AddPendingResourceHost(std::move(resource_host));
}
