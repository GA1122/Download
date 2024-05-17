MojoResult DataPipeConsumerDispatcher::Close() {
  base::AutoLock lock(lock_);
  DVLOG(1) << "Closing data pipe consumer " << pipe_id_;
  return CloseNoLock();
}
