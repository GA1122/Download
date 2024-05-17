  void DidDumpQuotaTable(QuotaManager* manager,
                         const DumpQuotaTableCallback& callback,
                         bool success) {
    if (!manager) {
      callback.Run(QuotaTableEntries());
      return;
    }
    manager->DidDatabaseWork(success);
    callback.Run(entries_);
  }
