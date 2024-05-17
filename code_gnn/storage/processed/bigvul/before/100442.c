void RenderThread::Resolve(const char* name, size_t length) {
  return dns_master_->Resolve(name, length);
}
