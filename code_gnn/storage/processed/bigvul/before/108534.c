void QuotaManagerProxy::RegisterClient(QuotaClient* client) {
  if (!io_thread_->BelongsToCurrentThread() &&
      io_thread_->PostTask(
          FROM_HERE,
          base::Bind(&QuotaManagerProxy::RegisterClient, this, client))) {
    return;
  }

  if (manager_)
    manager_->RegisterClient(client);
  else
    client->OnQuotaManagerDestroyed();
}
