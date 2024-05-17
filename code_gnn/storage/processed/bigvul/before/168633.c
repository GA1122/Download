void IndexedDBDatabase::SetIndexesReady(IndexedDBTransaction* transaction,
                                        int64_t,
                                        const std::vector<int64_t>& index_ids) {
  DCHECK(transaction);
  DCHECK_EQ(transaction->mode(), blink::kWebIDBTransactionModeVersionChange);

  transaction->ScheduleTask(
      blink::kWebIDBTaskTypePreemptive,
      base::BindOnce(&IndexedDBDatabase::SetIndexesReadyOperation, this,
                     index_ids.size()));
}
