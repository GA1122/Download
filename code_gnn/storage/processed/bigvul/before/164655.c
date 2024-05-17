void IndexedDBDatabase::ReportError(Status status) {
  DCHECK(!status.ok());
  if (status.IsCorruption()) {
    IndexedDBDatabaseError error(blink::kWebIDBDatabaseExceptionUnknownError,
                                 base::ASCIIToUTF16(status.ToString()));
    factory_->HandleBackingStoreCorruption(backing_store_->origin(), error);
  } else {
    factory_->HandleBackingStoreFailure(backing_store_->origin());
  }
}
