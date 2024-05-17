void BackendImpl::OnWrite(int32_t bytes) {
  OnRead(bytes);
}
