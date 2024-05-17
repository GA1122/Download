NetworkChangeNotifierMac::GetCurrentConnectionType() const {
  base::ThreadRestrictions::ScopedAllowWait allow_wait;
  base::AutoLock lock(connection_type_lock_);
  while (!connection_type_initialized_) {
    initial_connection_type_cv_.Wait();
  }
  return connection_type_;
}
