ChunkedUploadDataStream::CreateWriter() {
  return base::WrapUnique(new Writer(weak_factory_.GetWeakPtr()));
}
