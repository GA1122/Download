IndexedDBConnection::AddTransactionForTesting(
    std::unique_ptr<IndexedDBTransaction> transaction) {
  DCHECK(!base::ContainsKey(transactions_, transaction->id()));
  base::WeakPtr<IndexedDBTransaction> transaction_ptr =
      transaction->ptr_factory_.GetWeakPtr();
  transactions_[transaction->id()] = std::move(transaction);
  return transaction_ptr;
}
