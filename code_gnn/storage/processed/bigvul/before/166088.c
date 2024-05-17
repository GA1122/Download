void FileSystemManagerImpl::RemoveOpListener(OperationListenerID listener_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(op_listeners_.find(listener_id) != op_listeners_.end());
  op_listeners_.erase(listener_id);
}
