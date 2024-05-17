void BlobURLRequestJob::DidStart() {
  if (request()->method() != "GET") {
    NotifyFailure(net::ERR_METHOD_NOT_SUPPORTED);
    return;
  }

  if (!blob_data_) {
    NotifyFailure(net::ERR_FILE_NOT_FOUND);
    return;
  }

   CountSize();
 }
