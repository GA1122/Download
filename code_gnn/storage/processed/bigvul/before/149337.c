int64_t IndexedDBConnection::NewObserverTransactionId() {
  if (next_observer_transaction_id_ == 0)
    next_observer_transaction_id_ = 1;
  return static_cast<int64_t>(next_observer_transaction_id_++) << 32;
}
