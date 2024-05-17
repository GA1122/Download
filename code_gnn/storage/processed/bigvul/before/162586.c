void Resource::Trace(blink::Visitor* visitor) {
  visitor->Trace(loader_);
  visitor->Trace(cache_handler_);
  visitor->Trace(clients_);
  visitor->Trace(clients_awaiting_callback_);
  visitor->Trace(finished_clients_);
  visitor->Trace(finish_observers_);
  MemoryCoordinatorClient::Trace(visitor);
}
