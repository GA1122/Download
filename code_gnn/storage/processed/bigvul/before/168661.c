void IndexedDBTransaction::Timeout() {
  Abort(IndexedDBDatabaseError(
      blink::kWebIDBDatabaseExceptionTimeoutError,
      base::ASCIIToUTF16("Transaction timed out due to inactivity.")));
}
