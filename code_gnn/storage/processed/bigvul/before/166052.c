FileSystemManagerImpl::OperationListenerID FileSystemManagerImpl::AddOpListener(
    blink::mojom::FileSystemOperationListenerPtr listener) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  int op_id = next_operation_listener_id_++;
  listener.set_connection_error_handler(
      base::BindOnce(&FileSystemManagerImpl::OnConnectionErrorForOpListeners,
                     base::Unretained(this), op_id));
  op_listeners_[op_id] = std::move(listener);
  return op_id;
}
