void FileSystemManagerImpl::OnConnectionErrorForOpListeners(
    OperationListenerID listener_id) {
  RemoveOpListener(listener_id);
}
