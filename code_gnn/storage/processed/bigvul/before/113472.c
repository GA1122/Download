InputMethodIBus::PendingCreateICRequestImpl::~PendingCreateICRequestImpl() {
  if (request_ptr_) {
    DCHECK_EQ(*request_ptr_, this);
    *request_ptr_ = NULL;
  }
}
