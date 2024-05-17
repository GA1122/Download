void VirtualAuthenticator::GetUniqueId(GetUniqueIdCallback callback) {
  std::move(callback).Run(unique_id_);
}
