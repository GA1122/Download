void BlobURLRequestJob::Kill() {
  DeleteCurrentFileReader();

  net::URLRequestJob::Kill();
  weak_factory_.InvalidateWeakPtrs();
}
