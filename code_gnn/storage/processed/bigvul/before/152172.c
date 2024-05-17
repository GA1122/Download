void ServiceWorkerContextCore::UnregisterProviderHostByClientID(
    const std::string& client_uuid) {
  DCHECK(ContainsKey(*provider_by_uuid_, client_uuid));
  provider_by_uuid_->erase(client_uuid);
}
