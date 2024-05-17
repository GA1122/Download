DatabaseImpl::~DatabaseImpl() {
  idb_runner_->DeleteSoon(FROM_HERE, helper_);
}
