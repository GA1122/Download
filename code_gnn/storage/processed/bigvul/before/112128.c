 void SyncManager::ThrowUnrecoverableError() {
   DCHECK(thread_checker_.CalledOnValidThread());
   ReadTransaction trans(FROM_HERE, GetUserShare());
  trans.GetWrappedTrans()->OnUnrecoverableError(
      FROM_HERE, "Simulating unrecoverable error for testing purposes.");
}
