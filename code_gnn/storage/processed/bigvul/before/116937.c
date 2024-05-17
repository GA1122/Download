void IndexedDBDispatcher::OnComplete(int32 thread_id, int32 transaction_id) {
  DCHECK_EQ(thread_id, CurrentWorkerId());
  WebIDBTransactionCallbacks* callbacks =
      pending_transaction_callbacks_.Lookup(transaction_id);
  if (!callbacks)
    return;
  callbacks->onComplete();
  pending_transaction_callbacks_.Remove(transaction_id);
}
