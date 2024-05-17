void ImageLoader::DecodeRequestFinished(uint64_t request_id, bool success) {
  for (auto* it = decode_requests_.begin(); it != decode_requests_.end();
       ++it) {
    auto& request = *it;
    if (request->request_id() != request_id)
      continue;

    if (success)
      request->Resolve();
    else
      request->Reject();
    decode_requests_.erase(it);
    break;
  }
}
