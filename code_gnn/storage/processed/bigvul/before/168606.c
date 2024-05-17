std::tuple<scoped_refptr<IndexedDBDatabase>, Status> IndexedDBDatabase::Create(
    const base::string16& name,
    scoped_refptr<IndexedDBBackingStore> backing_store,
    scoped_refptr<IndexedDBFactory> factory,
    std::unique_ptr<IndexedDBMetadataCoding> metadata_coding,
    const Identifier& unique_identifier) {
  scoped_refptr<IndexedDBDatabase> database =
      IndexedDBClassFactory::Get()->CreateIndexedDBDatabase(
          name, backing_store, factory, std::move(metadata_coding),
          unique_identifier);
  Status s = database->OpenInternal();
  if (!s.ok())
    database = nullptr;
  return std::tie(database, s);
}
