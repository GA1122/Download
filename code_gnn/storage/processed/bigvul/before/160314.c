void ImageLoader::RejectPendingDecodes(UpdateType update_type) {
  for (auto* it = decode_requests_.begin(); it != decode_requests_.end();) {
    auto& request = *it;
    if (update_type == UpdateType::kAsync &&
        request->state() == DecodeRequest::kPendingMicrotask) {
      ++it;
      continue;
    }
    request->Reject();
    it = decode_requests_.erase(it);
  }
}
