void QuotaManager::RegisterClient(QuotaClient* client) {
  DCHECK(!database_.get());
  clients_.push_back(client);
}
