ServiceManagerConnectionImpl::~ServiceManagerConnectionImpl() {
  context_->ShutDown();
}
