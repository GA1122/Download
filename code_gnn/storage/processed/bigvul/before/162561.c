void Resource::RemoveClient(ResourceClient* client) {
  CHECK(!is_add_remove_client_prohibited_);


  if (finished_clients_.Contains(client))
    finished_clients_.erase(client);
  else if (clients_awaiting_callback_.Contains(client))
    clients_awaiting_callback_.erase(client);
  else
    clients_.erase(client);

  if (clients_awaiting_callback_.IsEmpty() &&
      async_finish_pending_clients_task_.IsActive()) {
    async_finish_pending_clients_task_.Cancel();
  }

  DidRemoveClientOrObserver();
}
