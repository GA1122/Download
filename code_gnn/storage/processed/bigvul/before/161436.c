Storage::StorageType GetTypeName(storage::QuotaClient::ID id) {
  switch (id) {
    case storage::QuotaClient::kFileSystem:
      return Storage::StorageTypeEnum::File_systems;
    case storage::QuotaClient::kDatabase:
      return Storage::StorageTypeEnum::Websql;
    case storage::QuotaClient::kAppcache:
      return Storage::StorageTypeEnum::Appcache;
    case storage::QuotaClient::kIndexedDatabase:
      return Storage::StorageTypeEnum::Indexeddb;
    case storage::QuotaClient::kServiceWorkerCache:
      return Storage::StorageTypeEnum::Cache_storage;
    case storage::QuotaClient::kServiceWorker:
      return Storage::StorageTypeEnum::Service_workers;
    default:
      return Storage::StorageTypeEnum::Other;
  }
}
