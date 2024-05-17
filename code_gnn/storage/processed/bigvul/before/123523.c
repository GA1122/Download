void BlobURLRequestJob::Start() {
  MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&BlobURLRequestJob::DidStart, weak_factory_.GetWeakPtr()));
}
