void IndexedDBDatabase::SetIndexesReady(IndexedDBTransaction* transaction,
                                        int64_t,
                                        const std::vector<int64_t>& index_ids) {
  DCHECK(transaction);
  DCHECK_EQ(transaction->mode(),
            blink::mojom::IDBTransactionMode::VersionChange);

  transaction->ScheduleTask(
      blink::mojom::IDBTaskType::Preemptive,
      base::BindOnce(&IndexedDBDatabase::SetIndexesReadyOperation, this,
                     index_ids.size()));
}
