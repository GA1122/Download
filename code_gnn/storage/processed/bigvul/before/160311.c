void ImageLoader::DecodeRequest::NotifyDecodeDispatched() {
  DCHECK_EQ(state_, kPendingLoad);
  state_ = kDispatched;
}
