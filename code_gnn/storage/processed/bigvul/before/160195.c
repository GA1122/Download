void MemBackendImpl::SetPostCleanupCallback(base::OnceClosure cb) {
  DCHECK(post_cleanup_callback_.is_null());
  post_cleanup_callback_ = std::move(cb);
}
