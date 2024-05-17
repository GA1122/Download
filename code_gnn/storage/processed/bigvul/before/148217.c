CursorImpl::~CursorImpl() {
  idb_runner_->DeleteSoon(FROM_HERE, helper_);
}
