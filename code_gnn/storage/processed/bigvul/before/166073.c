FileSystemManagerImpl::FileSystemManagerImpl(
    int process_id,
    int frame_id,
    storage::FileSystemContext* file_system_context,
    scoped_refptr<ChromeBlobStorageContext> blob_storage_context)
    : process_id_(process_id),
      frame_id_(frame_id),
      context_(file_system_context),
      security_policy_(ChildProcessSecurityPolicyImpl::GetInstance()),
      blob_storage_context_(blob_storage_context),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(context_);
  DCHECK(blob_storage_context);
  bindings_.set_connection_error_handler(base::BindRepeating(
      &FileSystemManagerImpl::OnConnectionError, base::Unretained(this)));
}
