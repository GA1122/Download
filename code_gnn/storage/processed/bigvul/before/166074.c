blink::mojom::FileSystemOperationListener* FileSystemManagerImpl::GetOpListener(
    OperationListenerID listener_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (op_listeners_.find(listener_id) == op_listeners_.end())
    return nullptr;
  return &*op_listeners_[listener_id];
}
