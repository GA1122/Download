void QuotaManager::DumpOriginInfoTable(
    const DumpOriginInfoTableCallback& callback) {
  DumpOriginInfoTableHelper* helper = new DumpOriginInfoTableHelper;
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&DumpOriginInfoTableHelper::DumpOriginInfoTableOnDBThread,
                 base::Unretained(helper)),
      base::Bind(&DumpOriginInfoTableHelper::DidDumpOriginInfoTable,
                 base::Owned(helper),
                 weak_factory_.GetWeakPtr(),
                 callback));
}
