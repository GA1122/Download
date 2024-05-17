void Resource::FinishPendingClients() {
  HeapVector<Member<ResourceClient>> clients_to_notify;
  CopyToVector(clients_awaiting_callback_, clients_to_notify);

  for (const auto& client : clients_to_notify) {
    if (!clients_awaiting_callback_.erase(client))
      continue;
    clients_.insert(client);

    if (!is_revalidating_)
      DidAddClient(client);
  }

  bool scheduled = async_finish_pending_clients_task_.IsActive();
  if (scheduled && clients_awaiting_callback_.IsEmpty())
    async_finish_pending_clients_task_.Cancel();

  DCHECK(clients_awaiting_callback_.IsEmpty() || scheduled);
}
