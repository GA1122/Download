void StoragePartitionImpl::WaitForDeletionTasksForTesting() {
  if (deletion_helpers_running_) {
    base::RunLoop loop;
    on_deletion_helpers_done_callback_ = loop.QuitClosure();
    loop.Run();
  }
}
