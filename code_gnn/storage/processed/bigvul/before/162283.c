void CommandBufferProxyImpl::RemoveDeletionObserver(
    DeletionObserver* observer) {
  std::unique_ptr<base::AutoLock> lock;
  if (lock_)
    lock.reset(new base::AutoLock(*lock_));
  deletion_observers_.RemoveObserver(observer);
}
