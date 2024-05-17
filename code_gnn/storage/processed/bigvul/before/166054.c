void FileSystemManagerImpl::Cancel(
    OperationID op_id,
    FileSystemCancellableOperationImpl::CancelCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  operation_runner()->Cancel(
      op_id, base::BindRepeating(&FileSystemManagerImpl::DidFinish,
                                 GetWeakPtr(), base::Passed(&callback)));
}
