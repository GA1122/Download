  void DidDumpOriginInfoTable(QuotaManager* manager,
                              const DumpOriginInfoTableCallback& callback,
                              bool success) {
    if (!manager) {
      callback.Run(OriginInfoTableEntries());
      return;
    }
    manager->DidDatabaseWork(success);
    callback.Run(entries_);
  }
